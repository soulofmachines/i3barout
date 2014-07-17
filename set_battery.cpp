#include <string.h>
#include <time.h>
#include "barconfig.hpp"
#include "file_to.hpp"
#include "set_icon.hpp"

using namespace std;

int set_battery (barconfig &myconfig) {
	bool fail;
	fail = false;
	string out, path, icon, status, width = "12:00 100%";
	long perc, e_now, e_full ,p_now, seconds;
	path = myconfig.device;
	path += "/capacity";
	perc = file_to_long (path.c_str(), fail);
	path = myconfig.device;
	path += "/status";
	status = file_to_string (path.c_str(), fail);
	path = myconfig.device;
	path += "/energy_now";
	e_now = file_to_long (path.c_str(), fail);
	path = myconfig.device;
	path += "/power_now";
	p_now = file_to_long (path.c_str(), fail);
	if (fail == true)
		return 0;
	if (p_now != 0)
		if (strcmp (status.c_str(), "Discharging\n") == 0)
			seconds = e_now * 3600 / p_now;
		else {
			path = myconfig.device;
			path += "/energy_full";
			e_full = file_to_long (path.c_str(), fail);
			if (fail == true)
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
	json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name));
	json_object_object_add(myconfig.json_output, "min_width", json_object_new_string (width.c_str()));
	json_object_object_add(myconfig.json_output, "align", json_object_new_string (myconfig.align));
	if (strcmp (status.c_str(), "Discharging\n") == 0) {
		if (perc <= myconfig.urgent) {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
			json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent));
		} else {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
			json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
			}
		set_icon_mask (myconfig, perc, 100);
	} else {
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
		set_icon_mask_zero (myconfig);
		}
	set_icon (myconfig);
	return 0;
}
