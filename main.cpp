#include <pwd.h>
#include <unistd.h>
#include <getopt.h>
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
	if (strcmp (value, (const char *)("ipv4")) == 0) return m_ipv4;
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

int line_setout (string &line_output) {
	if (line_output.size() > 0) {
		if (output.size () == 0)
			output = line_output;
		else {
			output += " | ";
			output += line_output;
			}
		line_output = "";
		}
	return 0;
	}


int main (int argc, char *argv[]) {
	bool infinite = true;
	bool background_input = true;
	bool onetime = false;
	bool json = true;
	ifstream configfile;
	stringstream ss;
	if (argc > 1) {
		int cmd;
		while ((cmd = getopt (argc, argv, "1c:l")) != -1)
			switch (cmd) {
				case '1':
					onetime = true;
					background_input = false;
					break;
				case 'c':
					configfile.open (optarg);
					if (!configfile.is_open())
						cerr << optarg << " not found" << endl;
					break;
				case 'l':
					json = false;
					background_input = false;
					break;
				};
		}
	vector <barconfig> myconfig;
	vector <input_exec> myinput;
	int lines = 0;
	if (!configfile.is_open()) {
		struct passwd *pw = getpwuid (getuid());
		char *dir = pw->pw_dir;
		strcat (dir, "/.i3/i3barout.json");
		configfile.open (dir);
		}
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
			if (background_input)
				myinput.push_back(input_exec());
			myconfig[lines].json_output = json_object_new_object();
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
				if (strcmp (key, "offset") == 0)
					myconfig[lines].offset = json_object_get_int(val);
				if (json) {
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
					if (strcmp (key, "urgent") == 0)
						myconfig[lines].urgent = json_object_get_int(val);
					if (strcmp (key, "width") == 0)
						myconfig[lines].width = json_object_get_boolean(val);
				} else
					if (strcmp (key, "line_prefix") == 0)
						myconfig[lines].line_prefix = json_object_get_string(val);
				if (background_input) {
					if (strcmp (key, "name") == 0)
						myinput.back().name = json_object_get_string(val);
					if (strcmp (key, "exec1") == 0)
						myinput.back().exec1 = json_object_get_string(val);
					if (strcmp (key, "exec2") == 0)
						myinput.back().exec2 = json_object_get_string(val);
					if (strcmp (key, "exec3") == 0)
						myinput.back().exec3 = json_object_get_string(val);
					}
				}
			if ((background_input) && (myinput.back().name.size() == 0))
					myinput.pop_back();
			lines += 1;
			}
		}
	if (myinput.size() == 0)
		background_input = false;
	auto input = async (launch::async, get_input, myinput, background_input);
	if (json)
		cout << "{\"version\":1,\"click_events\":true}\n[\n[]," << endl;
	while (infinite) {
		output = "";
		for (int counter = 0; counter < lines; ++counter) {
			switch (myconfig[counter].mode) {
				case m_null:
					break;
				case m_asound:
					set_asound (myconfig[counter], json);
					break;
				case m_battery:
					set_battery (myconfig[counter], json);
					break;
				case m_hwmon:
					set_hwmon (myconfig[counter], json);
					break;
				case m_ipv4:
					set_ipv4 (myconfig[counter], json);
					break;
				case m_nvidia:
					set_nvidia (myconfig[counter], json);
					break;
				case m_time:
					set_time (myconfig[counter], json);
					break;
				case m_wlan:
					set_wlan (myconfig[counter], json);
					break;
				}
			if (json)
				json_setout (myconfig[counter].json_output);
			else
				line_setout (myconfig[counter].line_output);
			}
		if (json)
			cout << "[\n" << output << "\n]," << endl;
		else
			cout << output << endl;
		if (!onetime)
			set_pause (5);
		else
			infinite = false;
		}
	return 0;
}
