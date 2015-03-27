#include "pidget.hpp"
#include "string.hpp"
#include <dirent.h>
#include <fstream>
#include <sstream>

#include <iostream>

int pidGetPid(std::string name) {
    int pid = 0;
    int id;
    std::ifstream cmdFile;
    std::stringstream ss;
    DIR* dp;
    dirent* dirp;
    dp = opendir("/proc");
    if (dp != NULL) {
        while (pid < 1 && (dirp = readdir(dp))) {
            id = 0;
            if (stringToInt(dirp->d_name, id)) {
                if (id > 0) {
                    cmdFile.open(std::string("/proc/") + std::to_string(id) + std::string("/comm"));
                    if (cmdFile.is_open()) {
                        ss << cmdFile.rdbuf();
                        cmdFile.close();
                        if (ss.str().substr(0, ss.str().length() - 1) == name)
                            pid = id;
                        ss.str("");
                    }
                }
            }
        }
        closedir(dp);
    } else
        pid = -1;
    return pid;
}

std::string pidGetStatus(int pid, int &ok) {
    ok = 1;
    std::ifstream cmdFile;
    std::stringstream ss;
    unsigned int start;
    cmdFile.open(std::string("/proc/") + std::to_string(pid) + std::string("/stat"));
    if (cmdFile.is_open()) {
        ss << cmdFile.rdbuf();
        cmdFile.close();
        start = ss.str().find(")");
        if (start == std::string::npos) {
            ok = 3;
        }
        if (ss.str().length() > start + 3) {
            ok = 0;
            return ss.str().substr(start + 2, 1);
        } else {
            ok = 4;
        }
    } else {
        ok = 2;
    }
    return "";
}

std::string pidGetStatusError(int ok) {
    switch (ok) {
    case 1:
        return "undefined";
        break;
    case 2:
        return "open";
        break;
    case 3:
        return "format";
        break;
    case 4:
        return "too fast";
        break;
    default:
        break;
    }
    return "";
}
