#ifndef STRINGTO_HPP
#define STRINGTO_HPP

#include <string>

enum modes {
    m_null = 0,
    m_asound,
    m_batt,
    m_hwmon,
    m_ipv4,
    m_nvidia,
    m_time,
    m_wlan
};

bool stringToInt(std::string input, int &output);
bool stringToFloat(std::string input, float &output);
modes stringToMode(std::string input);

#endif //STRINGTO_HPP
