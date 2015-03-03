#include "classtime.hpp"
#include "jsonget.hpp"

classTime::classTime() {
    integer = 0;
}

void classTime::readCustomConfig(Json::Value &config) {
    size = jsonGetInt(config["size"], 32);
    if (size < 1)
        size = 1;
    format = jsonGetString(config["format"], "%d/%m/%y %H:%M:%S");
}

void classTime::update() {
    output.clear();
    output.resize(size);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(&output.at(0), size, format.c_str(), timeinfo);
    output = output.substr(0, output.find_last_not_of(char()) + 1);
}
