#include <iniparser.h>
#include <json-c/json.h>
#include <iostream>

using namespace std;

enum modes {
m_null = 0,
m_time,
m_asound,
m_hwmon,
m_nvidia,
m_wlan
};

class barconfig {
public:
string color, color_warn, color_urgent;
string icon_name;
string name, exec;
const char *device, *format, *card, *program;
int offset, urgent;
modes mode;
json_object *json_output;
};

#include <setpause.h>
#include <gettime.h>
#include <getasound.h>
#include <getwlan.h>
#include <getnvidia.h>
#include <gethwmon.h>

string output;

int json_setout (json_object *json_output) {
if (json_object_object_length (json_output) > 0) {
if (output.size () == 0)
output = json_object_to_json_string (json_output);
else {
output += ",\n";
output += json_object_to_json_string (json_output);
}
}
return 0;
}

int main () {
dictionary *ini;
ini = iniparser_load(".i3/config.ini");
int ini_nsec = 0;
while (iniparser_getsecname (ini, ini_nsec) != NULL) {
ini_nsec += 1;
}
barconfig myconfig[ini_nsec];
for (int counter = 0; counter < ini_nsec; ++counter) {
if (strcmp (iniparser_getsecname (ini, counter), "wlan") == 0) {
myconfig[counter].mode = m_wlan;
myconfig[counter].device = iniparser_getstring (ini, "wlan:device", (char *)("wlan0"));
}
if (strcmp (iniparser_getsecname (ini, counter), "time") == 0) {
myconfig[counter].mode = m_time;
myconfig[counter].format = iniparser_getstring (ini, "time:format", (char *)("%H:%M"));
}
if (strcmp (iniparser_getsecname (ini, counter), "hwmon") == 0) {
myconfig[counter].mode = m_hwmon;
myconfig[counter].device = iniparser_getstring (ini, "hwmon:device", NULL);
myconfig[counter].offset = iniparser_getint (ini, "hwmon:offset", 1000);
}
if (strcmp (iniparser_getsecname (ini, counter), "nvidia") == 0) {
myconfig[counter].mode = m_nvidia;
myconfig[counter].program = iniparser_getstring (ini, "nvidia:program", (char *)("nvidia-smi -q -d TEMPERATURE"));
myconfig[counter].format = iniparser_getstring (ini, "nvidia:format", (char *)("Gpu"));
myconfig[counter].offset = iniparser_getint (ini, "nvidia:offset", 30);
}
if (strcmp (iniparser_getsecname (ini, counter), "asound") == 0) {
myconfig[counter].mode = m_asound;
myconfig[counter].card = iniparser_getstring (ini, "asound:card", (char *)("default"));
myconfig[counter].device = iniparser_getstring (ini, "asound:device", (char *)("Master"));
}
}
cout << "{\"version\":1,\"click_events\":true}\n[\n[]," << endl;
while (true) {
output = "";
for (int counter = 0; counter < ini_nsec; ++counter) {
switch (myconfig[counter].mode) {
case m_wlan:
getwlan (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_time:
gettime (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_hwmon:
gethwmon (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_nvidia:
getnvidia (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_asound:
getasound (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
}
}
cout << "[\n" << output << "\n]," << endl;
setpause (5);
}
iniparser_freedict(ini);
return 0;
}
