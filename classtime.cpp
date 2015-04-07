#include "classtime.hpp"
#include "json.hpp"

classTime::classTime() {
}

void classTime::readCustomConfig(yajl_val &config) {
    size = jsonGetInt(config, "size", 32);
    if (size < 1)
        size = 1;
    format = jsonGetString(config, "format", "%d/%m/%y %H:%M:%S");
    urgent = 1;
}

void classTime::update() {
    output.resize(size);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(&output.at(0), size, format.c_str(), timeinfo);
    output = output.substr(0, output.find_last_not_of(char()) + 1);
}
