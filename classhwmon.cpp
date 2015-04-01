#include <fstream>
#include <sstream>
#include "classhwmon.hpp"
#include "json.hpp"
#include "file.hpp"
#include "string.hpp"
#include "exception.hpp"

classHwmon::classHwmon() {
}

void classHwmon::readCustomConfig(yajl_val &config) {
    monitor = jsonGetString(config, "monitor", "/sys/devices/virtual/hwmon/hwmon0/temp1_input");
    divider = jsonGetInt(config, "divider", 1000);
    if (divider < 1)
        divider = 1;
    padding = jsonGetInt(config, "padding", 0);
}

void classHwmon::update() {
    try {
        integer = fileToInt(monitor);
    }
    catch(errorExc &exc) {
        error = "Monitor: ";
        error += exc.what();
    }
    integer = integer / divider;
    output = std::to_string(integer);
    stringPadZero(output, padding);
    output += "°C";
}
