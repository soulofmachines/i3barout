#include "set_time.hpp"
#include <time.h>

using namespace std;

int set_time (bar_config &my_bar_config) {
    char buffer[128];
    time_t rawtime;
    tm *timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer, sizeof (buffer), my_bar_config.input.param.c_str(), timeinfo);
    my_bar_config.output.integer = 0;
    my_bar_config.output.output = buffer;
    return 0;
}
