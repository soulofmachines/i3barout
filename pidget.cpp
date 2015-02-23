#include "pidget.hpp"
#include "stringto.hpp"
#include <dirent.h>
#include <fstream>
#include <sstream>

#include <iostream>

int pidGetPid(std::string name) {
    int pid = -1;
    int id;
    std::ifstream cmdFile;
    std::stringstream ss;
    DIR* dp;
    dirent* dirp;
    dp = opendir("/proc");
    if (dp != NULL) {
        while (pid < 0 && (dirp = readdir(dp))) {
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
        pid = -2;
    return pid;
}

std::string pidGetStatus(int pid) {
    std::ifstream cmdFile;
    std::stringstream ss;
    unsigned int start;
    cmdFile.open(std::string("/proc/") + std::to_string(pid) + std::string("/stat"));
    if (cmdFile.is_open()) {
        ss << cmdFile.rdbuf();
        cmdFile.close();
        start = ss.str().find(")");
        if (start == std::string::npos) {
            return "-2";
        }
        if (ss.str().length() > start + 3) {
            return ss.str().substr(start + 2, 1);
        } else {
            return "-1";
        }
    } else {
        return "-1";
    }
}
