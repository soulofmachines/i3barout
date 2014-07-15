#include <pwd.h>
#include <iniparser.h>
#include <json-c/json.h>
#include <iostream>
#include "barconfig.hpp"
#include "set_var.hpp"

using namespace std;

string output;

modes str2modes (const char *value) {
	if (strcmp (value, (const char *)("asound")) == 0) return m_asound;
	if (strcmp (value, (const char *)("battery")) == 0) return m_battery;
	if (strcmp (value, (const char *)("hwmon")) == 0) return m_hwmon;
	if (strcmp (value, (const char *)("nvidia")) == 0) return m_nvidia;
	if (strcmp (value, (const char *)("time")) == 0) return m_time;
	if (strcmp (value, (const char *)("wlan")) == 0) return m_wlan;
	return m_null;
	}

string string2arg (string first, string second) {
	string value;
	value = first;
	value += second;
	return value;
	}

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
	int ini_nsec = 0;
	struct passwd *pw = getpwuid (getuid());
	char *dir = pw->pw_dir;
	strcat (dir, "/.i3/i3barout.ini");
	dictionary *ini;
	ini = iniparser_load(dir);
	while (iniparser_getsecname (ini, ini_nsec) != NULL)
		ini_nsec += 1;
	if (ini_nsec == 0) {
		ini = iniparser_load("/etc/i3/i3barout.ini");
		while (iniparser_getsecname (ini, ini_nsec) != NULL)
			ini_nsec += 1;
		if (ini_nsec == 0) {
			ini = iniparser_load("i3barout.ini");
			while (iniparser_getsecname (ini, ini_nsec) != NULL)
				ini_nsec += 1;
			if (ini_nsec == 0)
				return 0;
			}
		}
	string mode;
	barconfig myconfig[ini_nsec];
	for (int counter = 0; counter < ini_nsec; ++counter) {
		mode = iniparser_getsecname (ini, counter);
		myconfig[counter].mode = str2modes (iniparser_getstring (ini, string2arg (mode, ":mode").c_str(), (char *)("null")));
		if (myconfig[counter].mode != m_null) {
			myconfig[counter].json_output = json_object_new_object();
			myconfig[counter].align = iniparser_getstring (ini, string2arg (mode, ":align").c_str(), (char *)("center"));
			myconfig[counter].color = iniparser_getstring (ini, string2arg (mode, ":color").c_str(), (char *)("#ffffff"));
			myconfig[counter].color_urgent = iniparser_getstring (ini, string2arg (mode, ":color_urgent").c_str(), (char *)("#ff0000"));
			myconfig[counter].icon = iniparser_getstring (ini, string2arg (mode, ":icon").c_str(), NULL);
			myconfig[counter].icon_mask = iniparser_getstring (ini, string2arg (mode, ":icon_mask").c_str(), NULL);
			myconfig[counter].icon_ext = iniparser_getstring (ini, string2arg (mode, ":icon_ext").c_str(), (char *)(".xbm"));
			myconfig[counter].icon_count = iniparser_getint (ini, string2arg (mode, ":icon_count").c_str(), 1);
			myconfig[counter].card = iniparser_getstring (ini, string2arg (mode, ":card").c_str(), NULL);
			myconfig[counter].device = iniparser_getstring (ini, string2arg (mode, ":device").c_str(), NULL);
			myconfig[counter].format = iniparser_getstring (ini, string2arg (mode, ":format").c_str(), NULL);
			myconfig[counter].program = iniparser_getstring (ini, string2arg (mode, ":program").c_str(), NULL);
			myconfig[counter].offset = iniparser_getint (ini, string2arg (mode, ":offset").c_str(), 0);
			myconfig[counter].urgent = iniparser_getint (ini, string2arg (mode, ":urgent").c_str(), 0);
			myconfig[counter].name = iniparser_getstring (ini, string2arg (mode, ":name").c_str(), NULL);
			myconfig[counter].exec = iniparser_getstring (ini, string2arg (mode, ":exec").c_str(), NULL);
			}
		}
	cout << "{\"version\":1,\"click_events\":true}\n[\n[]," << endl;
	while (true) {
		output = "";
		for (int counter = 0; counter < ini_nsec; ++counter) {
			switch (myconfig[counter].mode) {
				case m_null:
					break;
				case m_asound:
					set_asound (myconfig[counter]);
					json_setout (myconfig[counter].json_output);
					break;
				case m_battery:
					set_battery (myconfig[counter]);
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
				case m_time:
					set_time (myconfig[counter]);
					json_setout (myconfig[counter].json_output);
					break;
				case m_wlan:
					set_wlan (myconfig[counter]);
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
