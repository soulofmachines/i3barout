#include "set_battery.hpp"
#include "file_to.hpp"
#include <string.h>
#include <time.h>

using namespace std;

int set_battery (bar_config &my_bar_config) {
    bool charging = false;
    int return_value = 99;
    string path, status, width;
    long e_now, e_full ,p_now, seconds;
    char buffer[128];
    tm *tpoint;
    path = my_bar_config.input.device + "/capacity";
    if (!file_to_int (path.c_str(), my_bar_config.output.integer)) {
        return_value = -1;
        goto close;
    }
    path = my_bar_config.input.device + "/power_now";
    if (!file_to_long (path.c_str(), p_now)) {
        return_value = 1;
        goto close;
    }
    if (p_now != 0) {
        path = my_bar_config.input.device + "/status";
        if (!file_to_string (path.c_str(), status)) {
            return_value = 2;
            goto close;
        }
        path = my_bar_config.input.device + "/energy_now";
        if (!file_to_long (path.c_str(), e_now)) {
            return_value = 3;
            goto close;
        }
        if (strcmp (status.c_str(), "Discharging\n") == 0)
            seconds = e_now * 3600 / p_now;
        else {
            charging = true;
            path = my_bar_config.input.device + "/energy_full";
            if (!file_to_long (path.c_str(), e_full)) {
                return_value = 4;
                goto close;
            }
            seconds = (e_full - e_now) * 3600 / p_now;
        }
    } else
        seconds = 0;
    tpoint = gmtime (&seconds);
    strftime (buffer, 128, "%H:%M", tpoint);
    my_bar_config.output.output = buffer;
    my_bar_config.output.output += " " + to_string (my_bar_config.output.integer) + "%";
    if (charging)
        my_bar_config.output.integer = -1;
    else
        my_bar_config.output.integer = 100 - my_bar_config.output.integer;
    return_value = 0;
close:
    return return_value;
}
