#include <time.h>

#ifndef GETTIME_H_

using namespace std;

int gettime (barconfig &myconfig) {
myconfig.json_output = json_object_new_object();
time_t rawtime;
struct tm *timeinfo;
char buffer[128];
time (&rawtime);
timeinfo = localtime (&rawtime);
strftime (buffer, 128, myconfig.format, timeinfo);
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (buffer));
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
return 0;
}

#endif
