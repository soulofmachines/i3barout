#include "classnvidia.hpp"
#include "json.hpp"
#include "pid.hpp"
#include "string.hpp"
#include "exception.hpp"
#include <fstream>
#include <sstream>

classNvidia::classNvidia() {
}

void classNvidia::readCustomConfig(yajl_val &config) {
    exec = jsonGetString(config, "exec", "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader");
    module = jsonGetString(config, "module", "nvidia");
    optimus = jsonGetBool(config, "optimus", false);
}

void classNvidia::update() {
    std::ifstream file;
    std::stringstream ss;
    reader.clear();
    file.open("/proc/modules");
    if (!file.is_open()) {
        error = "Module: open";
        return;
    }
    ss << file.rdbuf();
    file.close();
    if ((ss.str().find(module + " ") == std::string::npos)) {
        if (optimus) {
            color = colorNormal;
            output = "Disabled";
            return;
        } else {
            error = "Module: find";
            return;
        }
    }
    if (exec.empty()) {
        error = "Exec: empty";
        return;
    }
    pid = pidGetPid(exec.substr(0, exec.find(" ")));
    if (pid > 0) {
        try {
            status = pidGetStatus(pid);
        }
        catch(errorExc &exc) {
            error = "Status: ";
            error += exc.what();
        }
        if ((status == "S") || (status == "R")) {
            error = "Status: SR";
            return;
        } else {
            error = "Status: undefined";
            return;
        }
    }
    if (pid < 0) {
        error = "Pid: proc";
        return;
    }
    fd = popen(exec.c_str(), "r");
    if (fd != NULL) {
        while (!feof(fd)) {
            if (std::fgets(buffer, sizeof(buffer), fd) != NULL) {
                reader += buffer;
            }
        }
        pclose(fd);
        try {
            integer = stringToInt(reader);
        }
        catch(errorExc &exc) {
            error = "Exec: ";
            error += exc.what();
        }
        output = std::to_string(integer);
        stringPadZero(output, padding);
        output += "°C";
    } else {
        error = "Exec: execute";
        return;
    }
}
