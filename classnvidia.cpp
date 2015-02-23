#include <fstream>
#include <sstream>
#include "classnvidia.hpp"
#include "jsonget.hpp"
#include "pidget.hpp"
#include "stringto.hpp"

classNvidia::classNvidia() {
}

void classNvidia::readConfig(Json::Value &config) {
    exec = jsonGetString(config["exec"], "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader");
    module = jsonGetString(config["module"], "nvidia");
}

void classNvidia::update() {
    integer = 0;
    output.clear();
    error.clear();
    std::ifstream file;
    std::stringstream ss;
    file.open("/proc/modules");
    if (!file.is_open()) {
        error = "ifstream.open";
        return;
    }
    ss << file.rdbuf();
    file.close();
    if ((ss.str().find(module + " ") == std::string::npos)) {
        error = "stringstream.str.find";
        return;
    }
    if (exec.empty()) {
        error = "string.empty";
        return;
    }
    pid = pidGetPid(exec.substr(0, exec.find(" ")));
    if (pid > 0) {
        status = pidGetStatus(pid);
        if ((status == "S") || (status == "R")) {
            error = "execSR";
            return;
        } else {
            if (status == "-1") {
                error = "pidGetStatus";
                return;
            } else {
                error = "exec";
                return;
            }
        }
    }
    fd = popen(exec.c_str(), "r");
    if (fd != NULL) {
        while (!feof(fd)) {
            if (std::fgets(buffer, sizeof(buffer), fd) != NULL) {
                reader += buffer;
            }
        }
        pclose(fd);
        if (!stringToInt(reader, integer)) {
            error = "stringToInt";
            return;
        }
        output = std::to_string(integer) + "°C";
    } else {
        error = "popen";
        return;
    }
}
