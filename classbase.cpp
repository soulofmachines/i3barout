#include "classbase.hpp"
#include "jsonget.hpp"

void classBase::readConfig(Json::Value &config) {
    readCustomConfig(config);
}

void classBase::setColor() {
    if (colored) {
        color.clear();
        if (integer < 0) {
            color = colorWarning;
        }
    }
}

void classBase::setOutput(std::string input) {
    if (!json) {
        if (colored) {
        output = "\033[" + color + "m" + input + "\033[" + "m";
        }
    }
}

std::string classBase::show() {
    update();
    if (!output.empty()) {
        setColor();
        setOutput(output);
    }
    if (!error.empty()) {
        color = colorUrgent;
        setOutput(error);
    }
    return output;
}
