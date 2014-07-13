#include <iniparser.h>
#include <json-c/json.h>
#include <iostream>
#include "barconfig.h"
#include "set_var.h"

using namespace std;

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
myconfig[counter].align = iniparser_getstring (ini, "wlan:align", (char *)("center"));
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
myconfig[counter].align = iniparser_getstring (ini, "asound:align", (char *)("center"));
}
if (strcmp (iniparser_getsecname (ini, counter), "bat") == 0) {
myconfig[counter].json_output = json_object_new_object();
myconfig[counter].mode = m_bat;
myconfig[counter].device = iniparser_getstring (ini, "bat:device", NULL);
myconfig[counter].color = iniparser_getstring (ini, "bat:color", (char *)("#ffffff"));
myconfig[counter].urgent = iniparser_getint (ini, "bat:urgent", 20);
myconfig[counter].color_urgent = iniparser_getstring (ini, "bat:color_urgent", (char *)("#ff0000"));
myconfig[counter].icon = iniparser_getstring (ini, "bat:icon", NULL);
myconfig[counter].icon_mask = iniparser_getstring (ini, "bat:icon_mask", NULL);
myconfig[counter].icon_count = iniparser_getint (ini, "bat:icon_count", 1);
myconfig[counter].icon_ext = iniparser_getstring (ini, "bat:icon_ext", (char *)(".xbm"));
myconfig[counter].align = iniparser_getstring (ini, "bat:align", (char *)("center"));
}
}
cout << "{\"version\":1,\"click_events\":true}\n[\n[]," << endl;
while (true) {
output = "";
for (int counter = 0; counter < ini_nsec; ++counter) {
switch (myconfig[counter].mode) {
case m_wlan:
set_wlan (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_time:
set_time (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_hwmon:
set_hwmon (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_nvidia:
set_nvidia (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_asound:
set_asound (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
case m_bat:
set_battery (myconfig[counter]);
json_setout (myconfig[counter].json_output);
break;
}
}
cout << "[\n" << output << "\n]," << endl;
set_pause (5);
}
iniparser_freedict(ini);
return 0;
}
