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
m_wlan,
m_bat
};

class barconfig {
public:
char *color, *color_warn, *color_urgent;
char *icon, *icon_mask, *icon_ext;
string name, exec;
const char *device, *format, *card, *program;
int offset, urgent, icon_count;
modes mode;
json_object *json_output;
};

#include <setpause.h>
#include <gettime.h>
#include <getasound.h>
#include <getwlan.h>
#include <getnvidia.h>
#include <gethwmon.h>
#include <getbat.h>

string output;

int json_setout (json_object *json_output) {
if (json_object_object_length (json_output) > 0) {
if (output.size () == 0)
output = json_object_to_json_string (json_output);
else {
output += ",\n";
output += json_object_to_json_string (json_output);
}
json_object_object_foreach (json_output, key, val)
json_object_object_del (json_output, key);
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
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_wlan;
myconfig[counter].device = iniparser_getstring (ini, "wlan:device", (char *)("wlan0"));
myconfig[counter].color = iniparser_getstring (ini, "wlan:color", (char *)("#ffffff"));
myconfig[counter].icon = iniparser_getstring (ini, "wlan:icon", NULL);
}
if (strcmp (iniparser_getsecname (ini, counter), "time") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_time;
myconfig[counter].format = iniparser_getstring (ini, "time:format", (char *)("%H:%M"));
myconfig[counter].color = iniparser_getstring (ini, "time:color", (char *)("#ffffff"));
myconfig[counter].icon = iniparser_getstring (ini, "time:icon", NULL);
}
if (strcmp (iniparser_getsecname (ini, counter), "hwmon") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_hwmon;
myconfig[counter].device = iniparser_getstring (ini, "hwmon:device", NULL);
myconfig[counter].offset = iniparser_getint (ini, "hwmon:offset", 1000);
myconfig[counter].color = iniparser_getstring (ini, "hwmon:color", (char *)("#ffffff"));
myconfig[counter].color_urgent = iniparser_getstring (ini, "hwmon:color_urgent", (char *)("#ff0000"));
myconfig[counter].urgent = iniparser_getint (ini, "hwmon:urgent", 80);
myconfig[counter].icon = iniparser_getstring (ini, "hwmon:icon", NULL);
}
if (strcmp (iniparser_getsecname (ini, counter), "nvidia") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_nvidia;
myconfig[counter].program = iniparser_getstring (ini, "nvidia:program", (char *)("nvidia-smi -q -d TEMPERATURE"));
myconfig[counter].format = iniparser_getstring (ini, "nvidia:format", (char *)("Gpu"));
myconfig[counter].offset = iniparser_getint (ini, "nvidia:offset", 30);
myconfig[counter].color = iniparser_getstring (ini, "nvidia:color", (char *)("#ffffff"));
myconfig[counter].color_urgent = iniparser_getstring (ini, "nvidia:color_urgent", (char *)("#ff0000"));
myconfig[counter].urgent = iniparser_getint (ini, "nvidia:urgent", 80);
myconfig[counter].icon = iniparser_getstring (ini, "nvidia:icon", NULL);
}
if (strcmp (iniparser_getsecname (ini, counter), "asound") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_asound;
myconfig[counter].card = iniparser_getstring (ini, "asound:card", (char *)("default"));
myconfig[counter].device = iniparser_getstring (ini, "asound:device", (char *)("Master"));
myconfig[counter].color = iniparser_getstring (ini, "asound:color", (char *)("#ffffff"));
myconfig[counter].color_warn = iniparser_getstring (ini, "asound:color_warn", (char *)("#00ffff"));
myconfig[counter].icon = iniparser_getstring (ini, "asound:icon", NULL);
myconfig[counter].icon_mask = iniparser_getstring (ini, "asound:icon_mask", NULL);
myconfig[counter].icon_count = iniparser_getint (ini, "asound:icon_count", 1);
myconfig[counter].icon_ext = iniparser_getstring (ini, "asound:icon_ext", (char *)(".xbm"));
}
if (strcmp (iniparser_getsecname (ini, counter), "bat") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_bat;
myconfig[counter].device = iniparser_getstring (ini, "bat:device", NULL);
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
case m_bat:
getbat (myconfig[counter]);
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
