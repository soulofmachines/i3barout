#include <pwd.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <json-c/json.h>
#include <iostream>
#include <future>
#include "barconfig.hpp"
#include "set_var.hpp"
#include "get_input.hpp"

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

const char *string2arg (string first, string second) {
	string value;
	value = first;
	value += second;
	char *cvalue = (char *)(malloc (value.size()+1));
	strcpy (cvalue, value.c_str());
	return cvalue;
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
	vector <barconfig> myconfig;
	vector <string> input_name, input_exec1, input_exec2, input_exec3;
	int lines = 0;
	ifstream configfile;
	stringstream ss;
	struct passwd *pw = getpwuid (getuid());
	char *dir = pw->pw_dir;
	strcat (dir, "/.i3/i3barout.json");
	configfile.open (dir);
	if (!configfile.is_open())
		configfile.open ("/etc/i3/i3barout.json");
	if (!configfile.is_open())
		configfile.open ("i3barout.json");
	if (!configfile.is_open()) {
		cout << "i3barout.json not found" << endl;
		return 0;
		}
	ss << configfile.rdbuf();
	configfile.close();
	string config;
	json_object *jobj;
	json_tokener_error jerr;
	while (getline(ss, config)) {
		jobj = json_tokener_parse_verbose (config.c_str(), &jerr);
		if (jerr == json_tokener_success) {
			myconfig.push_back(barconfig());
			myconfig[lines].json_output = json_object_new_object();
			input_name.push_back ("");
			input_exec1.push_back ("");
			input_exec2.push_back ("");
			input_exec3.push_back ("");
			json_object_object_foreach(jobj, key, val) {
				if (strcmp (key, "mode") == 0)
					myconfig[lines].mode = str2modes (json_object_get_string(val));
				if (strcmp (key, "card") == 0)
					myconfig[lines].card = json_object_get_string(val);
				if (strcmp (key, "device") == 0)
					myconfig[lines].device = json_object_get_string(val);
				if (strcmp (key, "format") == 0)
					myconfig[lines].format = json_object_get_string(val);
				if (strcmp (key, "program") == 0)
					myconfig[lines].program = json_object_get_string(val);
				if (strcmp (key, "align") == 0)
					myconfig[lines].align = json_object_get_string(val);
				if (strcmp (key, "color") == 0)
					myconfig[lines].color = json_object_get_string(val);
				if (strcmp (key, "color_urgent") == 0)
					myconfig[lines].color_urgent = json_object_get_string(val);
				if (strcmp (key, "icon") == 0)
					myconfig[lines].icon = json_object_get_string(val);
				if (strcmp (key, "icon_mask") == 0)
					myconfig[lines].icon_mask = json_object_get_string(val);
				if (strcmp (key, "icon_ext") == 0)
					myconfig[lines].icon_ext = json_object_get_string(val);
				if (strcmp (key, "name") == 0)
					myconfig[lines].name = json_object_get_string(val);
				if (strcmp (key, "prefix") == 0)
					myconfig[lines].prefix = json_object_get_string(val);
				if (strcmp (key, "icon_count") == 0)
					myconfig[lines].icon_count = json_object_get_int(val);
				if (strcmp (key, "offset") == 0)
					myconfig[lines].offset = json_object_get_int(val);
				if (strcmp (key, "urgent") == 0)
					myconfig[lines].urgent = json_object_get_int(val);
				if (strcmp (key, "width") == 0)
					myconfig[lines].width = json_object_get_boolean(val);
				if (strcmp (key, "name") == 0)
					input_name[lines] = json_object_get_string(val);
				if (strcmp (key, "exec1") == 0)
					input_exec1[lines] = json_object_get_string(val);
				if (strcmp (key, "exec2") == 0)
					input_exec2[lines] = json_object_get_string(val);
				if (strcmp (key, "exec3") == 0)
					input_exec3[lines] = json_object_get_string(val);
				}
			lines += 1;
			}
		}
	auto input = async (launch::async, get_input, input_name, input_exec1, input_exec2, input_exec3);
	cout << "{\"version\":1,\"click_events\":true}\n[\n[]," << endl;
	while (true) {
		output = "";
		for (int counter = 0; counter < lines; ++counter) {
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
	return 0;
}
