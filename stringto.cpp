#include "stringto.hpp"

bool stringToInt(std::string input, int &output) {
    try {
        output = stoi(input);
    }
    catch (...) {
        return false;
    }
    return true;
}

modes stringToMode(std::string input) {
    if (input == "asound") return m_asound;
    if (input == "battery") return m_batt;
    if (input == "hwmon") return m_hwmon;
    if (input == "ipv4") return m_ipv4;
    if (input == "nvidia") return m_nvidia;
    if (input == "time") return m_time;
    if (input == "wlan") return m_wlan;
    return m_null;
}
