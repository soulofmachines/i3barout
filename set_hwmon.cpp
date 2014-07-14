#include <fstream>
#include <sstream>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

int set_hwmon (barconfig &myconfig) {
	string out;
	int temp;
	ifstream infile;
	infile.open (myconfig.device);
	stringstream ss;
	ss << infile.rdbuf();
	string file = ss.str();
	if (file.size() == 0)
		return 0;
	temp = stoi (file) / myconfig.offset;
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
