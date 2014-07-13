#include <time.h>
#include "barconfig.hpp"

using namespace std;

int set_time (barconfig &myconfig) {
time_t rawtime;
struct tm *timeinfo;
char buffer[128];
time (&rawtime);
timeinfo = localtime (&rawtime);
strftime (buffer, 128, myconfig.format, timeinfo);
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (buffer));
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
if (myconfig.icon != NULL) {
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
}
return 0;
}
