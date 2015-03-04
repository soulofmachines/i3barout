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
    output = label + input;
}

std::string classBase::show() {
    update();
    if (!error.empty()) {
        color = colorUrgent;
        setOutput(error);
    } else {
        if (!output.empty()) {
            setColor();
            setOutput(output);
        }
    }
    if (json) {
        return showJson();
    }
    if (tmux) {
        return showTmux();
    } else {
        return showTerm();
    }
}

std::string classBase::showJson() {
    jsonOutput.clear();
    if (colored) {
        jsonOutput["color"] = color;
        if (!icon.empty()) {
            jsonOutput["icon_color"] = color;
        }
    }
    jsonOutput["text"] = output;
    return writer.write(jsonOutput);
}

std::string classBase::showTerm() {
    if (colored) {
        return separator + "\033[" + color + "m" + output + "\033[" + "m";
    } else {
        return separator + output;
    }
}

std::string classBase::showTmux() {
    return separator + output;
}
