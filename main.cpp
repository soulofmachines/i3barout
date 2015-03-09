#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "classasound.hpp"
#include "classbatt.hpp"
#include "classhwmon.hpp"
#include "classipv4.hpp"
#include "classnvidia.hpp"
#include "classtime.hpp"
#include "classwlan.hpp"
#include "jsonget.hpp"
#include "stringto.hpp"

std::vector<classBase*> elementV;
std::vector<classAsound> asoundV;
std::vector<classBatt> battV;
std::vector<classHwmon> hwmonV;
std::vector<classIpv4> ipv4V;
std::vector<classNvidia> nvidiaV;
std::vector<classTime> timeV;
std::vector<classWlan> wlanV;

Json::Value configFile;
bool colored, json, tmux, loop;
std::string colorNormal, colorUrgent, colorWarning;

void help(char* arg) {
    std::cout << arg << " config.json" << std::endl;
}

bool readConfig(std::string file) {
    Json::Reader configReader;
    std::ifstream configStream;
    configStream.open(file);
    if (!configStream.is_open()) {
        std::cerr << file << ": cant't open" << std::endl;
        return false;
    }
    if (!configReader.parse(configStream, configFile, false)) {
        std::cerr << file << ": " << configReader.getFormattedErrorMessages() << std::endl;
        return false;
    }
    configStream.close();
    return true;
}

bool parseConfig() {
    std::string name;
    Json::Value element;
    colored = jsonGetBool(configFile["colored"], true);
    json = jsonGetBool(configFile["json"], false);
    tmux = jsonGetBool(configFile["tmux"], false);
    loop = jsonGetBool(configFile["loop"], false);
    if (colored) {
        if (json) {
            colorNormal = jsonGetString(configFile["colorNormal"], "#ffffff");
            colorUrgent = jsonGetString(configFile["colorUrgent"], "#ff0000");
            colorWarning = jsonGetString(configFile["colorWarning"], "#00ffff");
        } else {
            if (tmux) {
                colorNormal = jsonGetString(configFile["colorNormal"], "white");
                colorUrgent = jsonGetString(configFile["colorUrgent"], "red");
                colorWarning = jsonGetString(configFile["colorWarning"], "cyan");
            } else {
                colorNormal = jsonGetString(configFile["colorNormal"], "37");
                colorUrgent = jsonGetString(configFile["colorUrgent"], "31");
                colorWarning = jsonGetString(configFile["colorWarning"], "36");
            }
        }
    }
    for (unsigned int x = 0; x < configFile.getMemberNames().size(); ++x) {
        name = configFile.getMemberNames().at(x);
        element = configFile[name];
        if (element.isObject() && !element.isNull()) {
            if (!element["mode"].isString())
                continue;
            switch (stringToMode(element["mode"].asString())) {
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
            std::cout << elementV.back()->show() << std::endl;
        }
    }
    return true;
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
    return 0;
}
