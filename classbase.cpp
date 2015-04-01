#include "classbase.hpp"
#include "json.hpp"
#include "string.hpp"
#include <cstring>
#include <cmath>
#include "exception.hpp"

void classBase::readConfig(yajl_val &config) {
    if (colored) {
        colorNormal = jsonGetString(config, "colorNormal", colorNormal);
        colorUrgent = jsonGetString(config, "colorUrgent", colorUrgent);
        colorWarning = jsonGetString(config, "colorWarning", colorWarning);
    }
    urgentLow = false;
    urgent = jsonGetInt(config, "urgent", 80);
    label = jsonGetString(config, "label", "");
    icon = jsonGetString(config, "icon", "");
    if (icon.find("%") != std::string::npos) {
        try {
            iconMax = stringToInt(icon.substr(icon.find("%") + 1, 3));
            iconName = icon.substr(0, icon.find("%"));
            iconExt = icon.substr(icon.find_last_of("."));
        }
        catch(errorExc &exc) {
            iconMax = 0;
            iconName = "error";
            iconExt = "";
        }
    }
    readCustomConfig(config);
}

void classBase::setColor() {
    if (colored && color.empty()) {
        if (integer < 0) {
            color = colorWarning;
        } else {
            if (urgent > 0) {
                if (!urgentLow) {
                    if (integer > urgent) {
                        color = colorUrgent;
                    } else {
                        color = colorNormal;
                    }
                } else {
                    if (integer < urgent) {
                        color = colorUrgent;
                    } else {
                        color = colorNormal;
                    }
                }
            } else {
                color = colorNormal;
            }
        }
    }
}

void classBase::setOutput(std::string input) {
    output = label + input;
}

void classBase::setIcon() {
    if (!iconName.empty()) {
        if (integer < 0) {
            icon = iconName + std::to_string(0) + iconExt;
        } else {
            iconNum = (int)ceil(((float)integer * (float)iconMax) / (float)100);
            if (iconNum > iconMax) {
                iconNum = iconMax;
            }
            if (iconNum < 0) {
                iconNum = 0;
            }
            icon = iconName + std::to_string(iconNum) + iconExt;
        }
    }
}

std::string classBase::show() {
    if(colored) {
        color.clear();
    }
    output.clear();
    error.clear();
    integer = 0;
    update();
    setIcon();
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
        return "";
    }
    if (tmux) {
        return showTmux();
    } else {
        return showTerm();
    }
}

void classBase::jsonAdd(yajl_gen &jsonOutput) {
    show();
    yajl_gen_map_open(jsonOutput);
    jsonMapAddString(jsonOutput, "full_text", output);
    jsonMapAddString(jsonOutput, "name", name);
    if (!icon.empty()) {
        jsonMapAddString(jsonOutput, "icon", icon);
    }
    if (colored) {
        jsonMapAddString(jsonOutput, "color", color);
        if (!icon.empty()) {
            jsonMapAddString(jsonOutput, "icon_color", color);
        }
    }
    yajl_gen_map_close(jsonOutput);
}

std::string classBase::showTerm() {
    if (colored) {
        return "\033[" + color + "m" + output + "\033[" + "m";
    } else {
        return output;
    }
}

std::string classBase::showTmux() {
    if (colored) {
        return  "#[fg=" + color + "]" + output + "#[fg=default]";
    } else {
        return output;
    }
}
