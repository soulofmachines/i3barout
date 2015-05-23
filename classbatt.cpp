#include "classbatt.hpp"
#include "json.hpp"
#include "file.hpp"
#include "string.hpp"
#include "exception.hpp"

classBatt::classBatt() {
}

void classBatt::readCustomConfig(yajl_val &config) {
    urgentLow = true;
    urgent = jsonGetInt(config, "urgent", 20);
    device = jsonGetString(config, "device", "/sys/class/power_supply/BAT0");
    pluggable = jsonGetBool(config, "pluggable", true);
    if (padding == 0) {
        padding = 3;
    }
    time.resize(5);
}

void classBatt::update() {
    if (!fileExist(device + "/capacity")) {
        if (pluggable) {
            integer = -1;
            output = "Off";
        } else {
            error = "Plugged out";
        }
        return;
    } else {
        if (!battCapacity()) {
            return;
        }
        if (!battStatus()) {
            return;
        }
        if (!battTime()) {
            return;
        }
        output = std::to_string(capacity);
        stringPadZero(output, padding);
        output += "% " + time;
    }
}

bool classBatt::battCapacity() {
    try {
        capacity = fileToInt(device + "/capacity");
    }
    catch(errorExc &exc) {
        error = "Capacity: ";
        error += exc.what();
        return false;
    }
    return true;
}

bool classBatt::battStatus() {
    try {
        status = fileToString(device + "/status");
    }
    catch(errorExc &exc) {
        error = "Status: ";
        error += exc.what();
        return false;
    }
    if (status != "Discharging") {
        integer = -1;
    } else {
        integer = capacity;
    }
    return true;
}

bool classBatt::battTime() {
    if (fileExist(device + "/energy_now")) {
        try {
            energyNow = fileToFloat(device + "/energy_now");
            powerNow = fileToFloat(device + "/power_now");
            if (integer == -1) {
                try {
                    energyFull = fileToFloat(device + "/energy_full");
                }
                catch(...) {
                    energyFull = fileToFloat(device + "/energy_full_design");
                }
            }
        }
        catch(errorExc &exc) {
            error = "Time: ";
            error += exc.what();
            return false;
        }
    } else {
        try {
            voltageNow = fileToFloat(device + "/voltage_now");
            energyNow = fileToFloat(device + "/charge_now");
            powerNow = fileToFloat(device + "/current_now");
            energyNow = energyNow * voltageNow;
            powerNow = powerNow * voltageNow;
            if (integer == -1) {
                try {
                    energyFull = fileToFloat(device + "/charge_full");
                }
                catch(...) {
                    energyFull = fileToFloat(device + "/charge_full_design");
                }
                energyFull = energyFull * voltageNow;
            }
        }
        catch(errorExc &exc) {
            error = "Time: ";
            error += exc.what();
            return false;
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
