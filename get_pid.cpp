#include "get_pid.hpp"
#include <dirent.h>
#include <fstream>

using namespace std;

long long int get_pid (string name) {
    long long int pid = -1;
    char *spos[1];
    DIR *dp = opendir ("/proc");
    if (dp != NULL) {
        dirent * dirp;
        while (pid < 0 && (dirp = readdir(dp))) {
            strtol (dirp->d_name, spos, 0);
            if (*spos != dirp->d_name) {
                int id = atoi (dirp->d_name);
                if (id > 0) {
                    string cmdPath = string("/proc/") + dirp->d_name + "/comm";
                    ifstream cmdFile (cmdPath.c_str());
                    string cmdLine;
                    getline (cmdFile, cmdLine);
                    if (!cmdLine.empty()) {
                        if (cmdLine == name) {
                            pid = id;
                        }
                    }
                }
            }
        }
        closedir (dp);
    }
    return pid;
}

string get_pid_status (long long pid) {
    string return_value = "";
    FILE *fd = NULL;
    string path = "/proc/" + to_string(pid) + "/stat";
    long long int p_pid;
    char p_tcom[PATH_MAX];
    char p_state;
    fd = fopen (path.c_str(), "r");
    if (fd == NULL)
        goto close;
    fscanf (fd, "%lld ", &p_pid);
    fscanf (fd, "%s ", p_tcom);
    fscanf (fd, "%c ", &p_state);
    fclose (fd);
    return_value = p_state;
close:
    return return_value;
}
