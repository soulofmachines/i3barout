#include "string.hpp"
#include "exception.hpp"

int stringToInt(std::string input) {
    try {
        return stoi(input);
    }
    catch (...) {
        throw errorExc("error");
        return 0;
    }
}

float stringToFloat(std::string input) {
    try {
        return stof(input);
    }
    catch (...) {
        throw errorExc("error");
        return 0;
    }
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

void stringPadZero(std::string &input, unsigned int size) {
    if (input.size() < size) {
        input = std::string(size-input.size(), '0') + input;
    }
}
