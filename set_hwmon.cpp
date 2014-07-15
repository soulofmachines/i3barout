#include "barconfig.hpp"
#include "file_to.hpp"
#include "set_icon.hpp"

using namespace std;

int set_hwmon (barconfig &myconfig) {
	if (myconfig.offset <= 0)
		return 0;
	bool fail;
	fail = false;
	string out;
	long temp = file_to_long (myconfig.device, fail);
	if (fail == true)
		return 0;
	temp /= myconfig.offset;
	out = to_string (temp) + "°C";
	json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
	if (temp >= myconfig.urgent) {
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent));
	} else {
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
		}
	set_icon (myconfig);
	return 0;
}
