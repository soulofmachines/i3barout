#include <fstream>
#include <sstream>
#include "classhwmon.hpp"
#include "jsonget.hpp"
#include "fileto.hpp"

classHwmon::classHwmon() {
}

void classHwmon::readCustomConfig(Json::Value &config) {
    monitor = jsonGetString(config["monitor"], "/sys/devices/virtual/hwmon/hwmon0/temp1_input");
    divider = jsonGetInt(config["divider"], 1000);
    if (divider < 1)
        divider = 1;
}

void classHwmon::update() {
    integer = 0;
    output.clear();
    error.clear();
    if (!fileToInt(monitor, integer, ok)) {
        error = "Monitor: " + fileToIntError(ok);
        return;
    }
    integer = integer / divider;
    output = std::to_string(integer) + "°C";
}
