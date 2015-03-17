#include "classbatt.hpp"
#include "json.hpp"
#include "file.hpp"

classBatt::classBatt() {
}

void classBatt::readCustomConfig(yajl_val &config) {
    urgentLow = true;
    urgent = jsonGetInt(config, "urgent", 20);
    device = jsonGetString(config, "device", "/sys/class/power_supply/BAT0");
    pluggable = jsonGetBool(config, "pluggable", true);
    padded = jsonGetBool(config, "padded", true);
    padding = jsonGetInt(config, "padding", 3);
    time.resize(5);
}

void classBatt::update() {
    integer = 0;
    output.clear();
    if (!fileExist(device + "/uevent")) {
        if (pluggable) {
            output = "Plugged out";
        } else {
            error = "Plugged out";
        }
        return;
    } else {
        if (!battCapacity()) {
            error = "Capacity: " + fileToIntError(ok);
            return;
        }
        if (!battStatus()) {
            error = "Status: " + fileToStringError(ok);
            return;
        }
        if (!battTime()) {
            error = "Time: " + fileToIntError(ok);
            return;
        }
        output = std::to_string(capacity);
        if (padded) {
            if (output.size() < padding) {
                output = std::string(padding-output.size(),'0') + output;
            }
        }
        output += "% " + time;
    }
}

bool classBatt::battCapacity() {
    if (!fileToInt(device + "/capacity", capacity, ok)) {
        return false;
    }
    return true;
}

bool classBatt::battStatus() {
    if (!fileToString(device + "/status", status, ok)) {
        return false;
    }
    if (status.substr(0, status.length() - 1) != "Discharging") {
        integer = -1;
    } else {
        integer = capacity;
    }
    return true;
}

bool classBatt::battTime() {
    if (fileExist(device + "/energy_now")) {
        if (!fileToFloat(device + "/energy_now", energyNow, ok)) {
            return false;
        }
        if (!fileToFloat(device + "/power_now", powerNow, ok)) {
            return false;
        }
        if (integer == -1) {
            if (!fileToFloat(device + "/energy_full", energyFull, ok)) {
                if (!fileToFloat(device + "/energy_full_design", energyFull, ok)) {
                    return false;
                }
            }
        }
    } else {
        if (!fileToFloat(device + "/voltage_now", voltageNow, ok)) {
            return false;
        }
        if (!fileToFloat(device + "/charge_now", energyNow, ok)) {
            return false;
        }
        energyNow = energyNow * voltageNow;
        if (!fileToFloat(device + "/current_now", powerNow, ok)) {
            return false;
        }
        powerNow = powerNow * voltageNow;
        if (integer == -1) {
            if (!fileToFloat(device + "/charge_full", energyFull, ok)) {
                if (!fileToFloat(device + "/charge_full_design", energyFull, ok)) {
                    return false;
                }
            }
            energyFull = energyFull * voltageNow;
        }
    }
    if (powerNow != 0) {
        if (integer == -1) {
            seconds = long((energyFull - energyNow) * 3600 / powerNow);
        } else {
            seconds = long(energyNow * 3600 / powerNow);
        }
    } else {
        seconds = 0;
    }
    timeinfo = std::gmtime(&seconds);
    std::strftime(&time.at(0), 6, "%H:%M", timeinfo);
    return true;
}
