#include <iostream>
#include <vector>
#include <fstream>
#include "classasound.hpp"
#include "classbatt.hpp"
#include "classhwmon.hpp"
#include "classipv4.hpp"
#include "classnvidia.hpp"
#include "classtime.hpp"
#include "classwlan.hpp"
#include "json.hpp"
#include "stringto.hpp"
#include <sstream>
#include <csignal>

std::vector<classBase*> elementV;
std::vector<classAsound> asoundV;
std::vector<classBatt> battV;
std::vector<classHwmon> hwmonV;
std::vector<classIpv4> ipv4V;
std::vector<classNvidia> nvidiaV;
std::vector<classTime> timeV;
std::vector<classWlan> wlanV;

yajl_val configFile;
yajl_gen jsonOutput;
const unsigned char* jsonBuf;
size_t jsonLen;
std::string output;
bool colored, json, tmux, loop;
std::string colorNormal, colorUrgent, colorWarning;

void help(char* arg) {
    std::cout << arg << " config.json" << std::endl;
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
    if (colored) {
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
    }
    for (unsigned int x = 0; x < YAJL_GET_OBJECT(configFile)->len; ++x) {
        element = YAJL_GET_OBJECT(configFile)->values[x];
        if (YAJL_IS_OBJECT(element)) {
            switch (stringToMode(jsonGetString(element, "mode", ""))) {
            case m_asound:
                asoundV.push_back(classAsound());
                elementV.push_back(&asoundV.back());
                break;
            case m_batt:
                battV.push_back(classBatt());
                elementV.push_back(&battV.back());
                break;
            case m_hwmon:
                hwmonV.push_back(classHwmon());
                elementV.push_back(&hwmonV.back());
                break;
            case m_ipv4:
                ipv4V.push_back(classIpv4());
                elementV.push_back(&ipv4V.back());
                break;
            case m_nvidia:
                nvidiaV.push_back(classNvidia());
                elementV.push_back(&nvidiaV.back());
                break;
            case m_time:
                timeV.push_back(classTime());
                elementV.push_back(&timeV.back());
                break;
            case m_wlan:
                wlanV.push_back(classWlan());
                elementV.push_back(&wlanV.back());
                break;
            default:
                continue;
            }
            elementV.back()->colored = colored;
            elementV.back()->json = json;
            elementV.back()->tmux = tmux;
            if (colored) {
                elementV.back()->colorNormal = colorNormal;
                elementV.back()->colorUrgent = colorUrgent;
                elementV.back()->colorWarning = colorWarning;
            }
            elementV.back()->readConfig(element);
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
        yajl_gen_clear(jsonOutput);
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
    } else {
        std::cout << output << std::endl;
    }
}

void stop(int signum) {
    loop = false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        help(argv[0]);
        return 1;
    }
    if (!readConfig(argv[1])) {
        return 2;
    }
    if (!parseConfig()) {
        return 3;
    }
    signal(SIGINT, stop);
    signal(SIGTERM, stop);
    if (json) {
        yajl_gen_array_open(jsonOutput);
        yajl_gen_clear(jsonOutput);
        std::cout << "{\"version\":1}\n[" << std::endl;
    }
    do {
        show();
        sleep(5);
    } while (loop);
    if (json) {
        yajl_gen_array_close(jsonOutput);
    }
    return 0;
}
