#include "barconfig.hpp"
#include "file_to.hpp"
#include "set_icon.hpp"

using namespace std;

int set_hwmon (barconfig &myconfig, bool line) {
	if (myconfig.offset <= 0)
		return 0;
	myconfig.line_output = myconfig.line_prefix;
	bool fail;
	fail = false;
	string out = myconfig.prefix;
	long temp = file_to_long (myconfig.device.c_str(), fail);
	if (fail == true)
		return 0;
	temp /= myconfig.offset;
	out += to_string (temp) + "°C";
	if (!line) {
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
		json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
		if (temp >= myconfig.urgent) {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent.c_str()));
			json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent.c_str()));
		} else {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
			json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color.c_str()));
			}
	} else
		myconfig.line_output += out;
	set_icon (myconfig);
	return 0;
}
