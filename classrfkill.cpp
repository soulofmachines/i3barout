#include "classrfkill.hpp"
#include "file.hpp"
#include "exception.hpp"
#include "json.hpp"

classRfkill::classRfkill() {
}

void classRfkill::readCustomConfig(yajl_val &config) {
    device = jsonGetString(config, "device", "hci0");
    on = jsonGetString(config, "on", "on");
    off = jsonGetString(config, "off", "off");
    path = "/sys/class/rfkill/";
    urgent = 1;
}

void classRfkill::update() {
    DIR* dp;
    dirent* dirp;
    dp = opendir(path.c_str());
    if (dp != NULL) {
        while ((dirp = readdir(dp)) != NULL) {
            dname = dirp->d_name;
            if ((dname != ".") && (dname != "..")) {
                try {
                    name = fileToString(path + dname + "/name");
                }
                catch (errorExc &exc) {
                    error = "RF: name";
                }
                if (name == device) {
                    try {
                        state = fileToInt(path + dname + "/state");
                    }
                    catch (errorExc &exc) {
                        error = "RF: state";
                    }
                    switch (state) {
                    case 0:
                        integer = -1;
                        output = off;
                        break;
                    case 1:
                        output = on;
                        break;
                    default:
                        error = "RF: state";
                        break;
                    }
                }
            }
        }
    } else {
        error = "RF: open";
    }
    if ((output.empty()) && (error.empty())) {
        error = "RF: device";
    }
}
