#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

bool no_fail = TRUE;

int filetolong (const char *path, long &value) {
	char *spos[1];
	ifstream infile;
	stringstream ss;
	infile.open(path);
	if (!infile.is_open()) {
		no_fail = FALSE;
		return 0;
		}
	ss << infile.rdbuf();
	infile.close();
	string file = ss.str();
	strtol (file.c_str(), spos, 0);
	if (*spos == file.c_str()) {
		no_fail = FALSE;
		return 0;
		}
	value = stol (file);
	return 0;
	}

int filetostr (const char *path, string &value) {
	ifstream infile;
	stringstream ss;
	infile.open (path);
	if (!infile.is_open()) {
		no_fail = FALSE;
		return 0;
		}
	ss << infile.rdbuf();
	infile.close();
	value = ss.str();
	return 0;
	}

int set_battery (barconfig &myconfig) {
	no_fail = TRUE;
	string out, path, icon, status, width = "12:00 100%";
	long perc, e_now, e_full ,p_now, seconds;
	path = myconfig.device;
	path += "/capacity";
	filetolong (path.c_str(), perc);
	path = myconfig.device;
	path += "/status";
	filetostr (path.c_str(), status);
	path = myconfig.device;
	path += "/energy_now";
	filetolong (path.c_str(), e_now);
	path = myconfig.device;
	path += "/power_now";
	filetolong (path.c_str(), p_now);
	if (no_fail == FALSE)
		return 0;
	if (p_now != 0)
		if (strcmp (status.c_str(), "Discharging\n") == 0)
			seconds = e_now * 3600 / p_now;
		else {
			path = myconfig.device;
			path += "/energy_full";
			filetolong (path.c_str(), e_full);
			if (no_fail == FALSE)
				return 0;
			seconds = (e_full - e_now) * 3600 / p_now;
			}
		else
			seconds = 0;
	tm *tpoint = gmtime(&seconds);
	char buffer[128];
	strftime (buffer,128,"%H:%M",tpoint);
	out = buffer;
	out += " " + to_string (perc) + "%";
	json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
	json_object_object_add(myconfig.json_output, "min_width", json_object_new_string (width.c_str()));
	json_object_object_add(myconfig.json_output, "align", json_object_new_string (myconfig.align));
	if (perc <= myconfig.urgent) {
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent));
	} else {
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
		}
	set_icon (myconfig);
	set_icon_mask (myconfig, perc, 100);
	return 0;
}
