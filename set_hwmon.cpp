#include "set_hwmon.hpp"
#include "file_to.hpp"

using namespace std;

int set_hwmon (bar_config &my_bar_config) {
    int return_value = 99;
    if (my_bar_config.input.offset <= 0) {
        return_value = 1;
        goto close;
    }
    long temp;
    if (!file_to_long (my_bar_config.input.device.c_str(), temp)) {
        return_value = 2;
        goto close;
    }
    my_bar_config.output.integer = int (temp) / my_bar_config.input.offset;
    my_bar_config.output.output = to_string (my_bar_config.output.integer) + "°C";
    return_value = 0;
close:
    return return_value;
}
