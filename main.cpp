#include <iostream>
#include <unistd.h>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "classasound.hpp"
#include "classipv4.hpp"
#include "classtime.hpp"
#include "classwlan.hpp"
//#include "fileget.hpp"
#include "classhwmon.hpp"
#include "classbatt.hpp"
#include "pidget.hpp"
#include "classnvidia.hpp"

int main() {
    std::vector<classBase*> elements;
    std::vector<classTime> times;
    std::vector<classAsound> asounds;
    std::vector<classIpv4> ipv4s;
    std::vector<classWlan> wlans;
    std::vector<classHwmon> hwmons;
    std::vector<classBatt> batts;
    std::vector<classNvidia> nvidias;
    times.push_back(classTime());
    asounds.push_back(classAsound());
    ipv4s.push_back(classIpv4());
    wlans.push_back(classWlan());
    hwmons.push_back(classHwmon());
    batts.push_back(classBatt());
    nvidias.push_back(classNvidia());
    elements.push_back(&times.back());
    elements.push_back(&asounds.back());
    elements.push_back(&ipv4s.back());
    elements.push_back(&wlans.back());
    elements.push_back(&hwmons.back());
    elements.push_back(&batts.back());
    elements.push_back(&nvidias.back());
    Json::Value configFile;
    Json::Value configElement;
    Json::Reader configReader;
    std::ifstream configStream;
    std::string error;
    configStream.open("config.json");
    configReader.parse(configStream, configFile);
    configStream.close();
    //int test = -1;
    std::cout << configFile.getMemberNames().size() << std::endl;
    for (unsigned int x = 0; x < elements.size(); ++x) {
        elements.at(x)->json = false;
        elements.at(x)->tmux = false;
        elements.at(x)->colored = true;
        elements.at(x)->colorNormal = "37";
        elements.at(x)->colorUrgent = "31";
        elements.at(x)->colorWarning = "36";
    elements.at(x)->readConfig(configFile);
    }
    for (unsigned int x = 1; x < elements.size(); ++x) {
        elements.at(x)->separator = " | ";
    }
    while (true) {
//    for (int loop = 0; loop < 17280; ++loop) {
    for (unsigned int x = 0; x < elements.size(); ++x) {
        std::cout << elements.at(x)->show();
    }
    std::cout << std::endl;
    //fileGetInt("testnum", test, error);
    //std::cout << test << std::endl;
    //sleep(1);
}
    int ok;
    std::cout << pidGetPid("udevd") << std::endl;
    std::cout << pidGetStatus(pidGetPid("sh"), ok) << std::endl;
    return 0;
}
