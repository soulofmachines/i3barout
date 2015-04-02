#include "config.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#ifdef USE_ALSA
#include "classasound.hpp"
#endif //USE_ALSA
#include "classbatt.hpp"
#include "classhwmon.hpp"
#include "classipv4.hpp"
#include "classnvidia.hpp"
#include "classtime.hpp"
#include "classwlan.hpp"
#include "json.hpp"
#include "string.hpp"
#include <sstream>
#include <csignal>
#include "input.hpp"
#include <thread>
#include <cstring>
#include <unistd.h>

constexpr unsigned int stringToHash_const(const char* input, int x = 0) {
    return !input[x] ? 5381 : (stringToHash_const(input, x+1)*33) ^ input[x];
}

std::vector<classBase*> elementV;
#ifdef USE_ALSA
std::vector<classAsound*> asoundV;
#endif //USE_ALSA
std::vector<classBatt*> battV;
std::vector<classHwmon*> hwmonV;
std::vector<classIpv4*> ipv4V;
std::vector<classNvidia*> nvidiaV;
std::vector<classTime*> timeV;
std::vector<classWlan*> wlanV;
std::vector<classInput> inputV;

yajl_val configFile;
yajl_gen jsonOutput;
const unsigned char* jsonBuf;
size_t jsonLen;
std::string output;
bool colored, json, tmux, loop, input;
unsigned int timeout;
std::string colorNormal, colorUrgent, colorWarning;

void help(char* arg) {
    std::cout << arg << " config.json" << std::endl;
}

yajl_gen_status yajl_gen_val(yajl_gen g, yajl_val v)
{
    size_t i;
    const char* key;
    yajl_gen_status status;
    if (YAJL_IS_NULL(v))
        return yajl_gen_null(g);
    else if (YAJL_IS_TRUE(v))
        return yajl_gen_bool(g, 1);
    else if (YAJL_IS_FALSE(v))
        return yajl_gen_bool(g, 0);
    else if (YAJL_IS_NUMBER(v))
        return yajl_gen_number(g, v->u.number.r, strlen(v->u.number.r));
    else if (YAJL_IS_STRING(v))
        return yajl_gen_string(g, (const unsigned char*)v->u.string, strlen(v->u.string));
    else if (YAJL_IS_ARRAY(v)) {
        status = yajl_gen_array_open(g);
        if (status != yajl_gen_status_ok) return status;
        for (i = 0; i < YAJL_GET_ARRAY(v)->len; i++) {
            yajl_gen_val(g, YAJL_GET_ARRAY(v)->values[i]);
            if (status != yajl_gen_status_ok) return status;
        }
        return yajl_gen_array_close(g);
    }
    else if (YAJL_IS_OBJECT(v)) {
        status = yajl_gen_map_open(g);
        if (status != yajl_gen_status_ok) return status;
        for (i = 0; i < YAJL_GET_OBJECT(v)->len; i++) {
            key = YAJL_GET_OBJECT(v)->keys[i];
            status = yajl_gen_string(g, (const unsigned char*)key, strlen(key));
            if (status != yajl_gen_status_ok) return status;
            status = yajl_gen_val(g, YAJL_GET_OBJECT(v)->values[i]);
            if (status != yajl_gen_status_ok) return status;
        }
        return yajl_gen_map_close(g);
    }
    return yajl_gen_in_error_state;
}

bool formatConfig(std::string input, std::string output = "") {
    std::ifstream inputStream;
    std::ofstream outputStream;
    std::stringstream ss;
    char errbuf[1024];
    inputStream.open(input);
    if (!inputStream.is_open()) {
        std::cerr << input << ": cant't open" << std::endl;
        return false;
    }
    ss << inputStream.rdbuf();
    inputStream.close();
    configFile = yajl_tree_parse(ss.str().c_str(), errbuf, sizeof(errbuf));
    if (configFile == NULL) {
        if (strlen(errbuf)) {
            std::cerr << input << ": " << errbuf << std::endl;
        } else {
            std::cerr << input << ": " << "unknown error" << std::endl;
        }
        return false;
    }
    jsonOutput = yajl_gen_alloc(NULL);
    yajl_gen_config(jsonOutput, yajl_gen_beautify, 1);
    yajl_gen_val(jsonOutput, configFile);
    yajl_gen_get_buf(jsonOutput, &jsonBuf, &jsonLen);
    if (output.empty()) {
        std::cout << jsonBuf;
    } else {
        outputStream.open(output, std::ofstream::out);
        if (!outputStream.is_open()) {
            std::cerr << output << ": can't open" << std::endl;
            return false;
        }
        outputStream << jsonBuf;
        outputStream.close();
    }
    return true;
}

bool readConfig(std::string file) {
    std::ifstream configStream;
    std::stringstream ss;
    char errbuf[1024];
    configStream.open(file);
    if (!configStream.is_open()) {
        std::cerr << file << ": cant't open" << std::endl;
        return false;
    }
    ss << configStream.rdbuf();
    configStream.close();
    configFile = yajl_tree_parse(ss.str().c_str(), errbuf, sizeof(errbuf));
    if (configFile == NULL) {
        if (strlen(errbuf)) {
            std::cerr << file << ": " << errbuf << std::endl;
        } else {
            std::cerr << file << ": " << "unknown error" << std::endl;
        }
        return false;
    }
    if ((!YAJL_IS_OBJECT(configFile) || (YAJL_GET_OBJECT(configFile)->len == 0))) {
        std::cerr << file << ": " << "format error" << std::endl;
        return false;
    }
    return true;
}

bool parseConfig() {
    yajl_val element;
    colored = jsonGetBool(configFile, "colored", true);
    json = jsonGetBool(configFile, "json", false);
    tmux = jsonGetBool(configFile, "tmux", false);
    loop = jsonGetBool(configFile, "loop", false);
    input = jsonGetBool(configFile, "input", false);
    timeout = jsonGetInt(configFile, "timeout", 5);
    if (json) {
        colorNormal = jsonGetString(configFile, "colorNormal", "#ffffff");
        colorUrgent = jsonGetString(configFile, "colorUrgent", "#ff0000");
        colorWarning = jsonGetString(configFile, "colorWarning", "#00ffff");
    } else {
        if (tmux) {
            colorNormal = jsonGetString(configFile, "colorNormal", "white");
            colorUrgent = jsonGetString(configFile, "colorUrgent", "red");
            colorWarning = jsonGetString(configFile, "colorWarning", "cyan");
        } else {
            colorNormal = jsonGetString(configFile, "colorNormal", "37");
            colorUrgent = jsonGetString(configFile, "colorUrgent", "31");
            colorWarning = jsonGetString(configFile, "colorWarning", "36");
        }
    }
    for (unsigned int x = 0; x < YAJL_GET_OBJECT(configFile)->len; ++x) {
        element = YAJL_GET_OBJECT(configFile)->values[x];
        if (YAJL_IS_OBJECT(element)) {
            switch (stringToHash(jsonGetString(element, "mode", "").c_str())) {
#ifdef USE_ALSA
            case stringToHash_const("asound"):
                asoundV.push_back(new classAsound());
                elementV.push_back(asoundV.back());
                break;
#endif //USE_ALSA
            case stringToHash_const("battery"):
                battV.push_back(new classBatt());
                elementV.push_back(battV.back());
                break;
            case stringToHash_const("hwmon"):
                hwmonV.push_back(new classHwmon());
                elementV.push_back(hwmonV.back());
                break;
            case stringToHash_const("ipv4"):
                ipv4V.push_back(new classIpv4());
                elementV.push_back(ipv4V.back());
                break;
            case stringToHash_const("nvidia"):
                nvidiaV.push_back(new classNvidia());
                elementV.push_back(nvidiaV.back());
                break;
            case stringToHash_const("time"):
                timeV.push_back(new classTime());
                elementV.push_back(timeV.back());
                break;
            case stringToHash_const("wlan"):
                wlanV.push_back(new classWlan());
                elementV.push_back(wlanV.back());
                break;
            default:
                continue;
            }
            elementV.back()->name = "element" + std::to_string(x);
            elementV.back()->colored = colored;
            elementV.back()->json = json;
            elementV.back()->tmux = tmux;
            elementV.back()->colorNormal = colorNormal;
            elementV.back()->colorUrgent = colorUrgent;
            elementV.back()->colorWarning = colorWarning;
            elementV.back()->readConfig(element);
            if (loop && json && input) {
                inputV.push_back(classInput());
                inputV.back().name = elementV.back()->name;
                inputV.back().exec1 = jsonGetString(element, "exec1", "");
                inputV.back().exec2 = jsonGetString(element, "exec2", "");
                inputV.back().exec3 = jsonGetString(element, "exec3", "");
                if (!(!inputV.back().exec1.empty() || !inputV.back().exec2.empty() || !inputV.back().exec3.empty())) {
                    inputV.pop_back();
                }
            }
            element = NULL;
        }
    }
    yajl_tree_free(configFile);
    if (elementV.size() == 0) {
        std::cerr << "config empty" << std::endl;
        return false;
    }
    if (json) {
        jsonOutput = yajl_gen_alloc(NULL);
    }
    return true;
}

void show() {
    if (json) {
        yajl_gen_array_open(jsonOutput);
    } else {
        output.clear();
    }
    for (unsigned int x = 0; x < elementV.size(); ++x) {
        if (json) {
            elementV.at(x)->jsonAdd(jsonOutput);
        } else {
            output += elementV.at(x)->show();
            if (x != elementV.size() - 1) {
                output += " | ";
            }
        }
    }
    if (json) {
        yajl_gen_array_close(jsonOutput);
        yajl_gen_get_buf(jsonOutput, &jsonBuf, &jsonLen);
        std::cout << jsonBuf << std::endl;
        yajl_gen_clear(jsonOutput);
    } else {
        std::cout << output << std::endl;
    }
}

void stop(int signum) {
    loop = false;
}

void unpause(int signum) {
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        help(argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-h") == 0) {
        help(argv[0]);
        return 0;
    }
    if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3) {
            help(argv[0]);
            return 1;
        }
        if (argc == 3) {
            if (!formatConfig(argv[2]))
                return 1;
        } else {
            if(!formatConfig(argv[2], argv[3]))
                return 1;
        }
        return 0;
    }
    if (!readConfig(argv[1])) {
        return 2;
    }
    if (!parseConfig()) {
        return 3;
    }
    signal(SIGINT, stop);
    signal(SIGTERM, stop);
    signal(SIGUSR1, unpause);
    if (json) {
        yajl_gen_array_open(jsonOutput);
        if (input) {
            std::thread inputThread(inputProtocol, inputV);
            inputThread.detach();
            std::cout << "{\"version\":1,\"click_events\":true}" << std::endl;
        } else {
            std::cout << "{\"version\":1}" << std::endl;
        }
    }
    do {
        show();
        if (loop) {
            sleep(timeout);
        }
    } while (loop);
    if (json) {
        yajl_gen_array_close(jsonOutput);
        yajl_gen_get_buf(jsonOutput, &jsonBuf, &jsonLen);
        std::cout << jsonBuf << std::endl;
    }
    return 0;
}
