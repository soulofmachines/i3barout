#include "classbatt.hpp"
#include "json.hpp"
#include "fileto.hpp"

classBatt::classBatt() {
}

void classBatt::readCustomConfig(yajl_val &config) {
    device = jsonGetString(config, "device", "/sys/class/power_supply/BAT0");
    time.resize(5);
}

void classBatt::update() {
    integer = 0;
    output.clear();
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
    output = time + " " + std::to_string(capacity) + "%";
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
    if (!fileToInt(device + "/power_now", powerNow, ok)) {
        return false;
    }
    if (powerNow != 0) {
        if (!fileToInt(device + "/energy_now", energyNow, ok)) {
            return false;
        }
        if (integer != -1)
            seconds = long(energyNow) * 3600 / long(powerNow);
        else {
            if (!fileToInt(device + "/energy_full", energyFull, ok)) {
                return false;
            }
            seconds = long(energyFull - energyNow) * 3600 / long(powerNow);
        }
    } else
        seconds = 0;
    timeinfo = std::gmtime(&seconds);
    std::strftime(&time.at(0), 6, "%H:%M", timeinfo);
    return true;
}
