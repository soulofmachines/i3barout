#include <fstream>
#include <sstream>
#include "classnvidia.hpp"
#include "jsonget.hpp"
#include "pidget.hpp"
#include "stringto.hpp"

classNvidia::classNvidia() {
}

void classNvidia::readCustomConfig(yajl_val &config) {
    exec = jsonGetString(config, "exec", "nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader");
    module = jsonGetString(config, "module", "nvidia");
}

void classNvidia::update() {
    integer = 0;
    output.clear();
    error.clear();
    reader.clear();
    std::ifstream file;
    std::stringstream ss;
    file.open("/proc/modules");
    if (!file.is_open()) {
        error = "Module: open";
        return;
    }
    ss << file.rdbuf();
    file.close();
    if ((ss.str().find(module + " ") == std::string::npos)) {
        error = "Module: find";
        return;
    }
    if (exec.empty()) {
        error = "Exec: empty";
        return;
    }
    pid = pidGetPid(exec.substr(0, exec.find(" ")));
    if (pid > 0) {
        status = pidGetStatus(pid, ok);
        if ((status == "S") || (status == "R")) {
            error = "Status: SR";
            return;
        } else {
            if (status.empty()) {
                error = "Status: " + pidGetStatusError(ok);
                return;
            } else {
                error = "Status: undefined";
                return;
            }
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
        if (!stringToInt(reader, integer)) {
            error = "Exec: output";
            return;
        }
        output = std::to_string(integer) + "°C";
    } else {
        error = "Exec: execute";
        return;
    }
}
