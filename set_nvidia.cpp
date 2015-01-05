#include "set_nvidia.hpp"
#include "get_pid.hpp"
#include "string_to.hpp"
#include <sstream>
#include <fstream>

using namespace std;

int set_nvidia (bar_config &my_bar_config) {
    int return_value = 99;
    ifstream infile;
    FILE *fd = NULL;
    char buffer[128];
    infile.open ("/proc/modules");
    stringstream ss;
    ss << infile.rdbuf();
    string file = ss.str(), program;
    size_t space;
    long int pid = -1;
    string state = "";
    if ((file.find ("\nnvidia ") != string::npos) || (file.find ("nvidia ") == 0)) {
        space = my_bar_config.input.device.find_first_of(" ");
        if (space != string::npos) {
            program = my_bar_config.input.device.substr(0,space);
        } else {
            program = my_bar_config.input.device;
        }
        if (program.size() != 0) {
            pid = get_pid (program);
            if (pid != -1) {
                state = get_pid_status (pid);
                if (state == "R" || state == "S") {
                    return_value = 1;
                    goto close;
                } else {
                    return_value = 2;
                    goto close;
                }
            }
        } else {
            return_value = 2;
            goto close;
        }
        fd = popen (my_bar_config.input.device.c_str(), "r");
        if (fd == NULL) {
            return_value = 3;
            goto close;
        }
        fgets (buffer, sizeof (buffer), fd);
        if (string_to_int (buffer, my_bar_config.output.integer))
            my_bar_config.output.output = to_string (my_bar_config.output.integer) + "°C";
        else {
            return_value = 4;
            goto close;
        }
        return_value = 0;
    } else {
        return_value = -1;
    }
close:
    if (fd != NULL)
        pclose (fd);
    return return_value;
}
