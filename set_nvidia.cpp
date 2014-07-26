#include <fstream>
#include <sstream>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

long string_to_long (string value, bool &fail) {
	char *spos[1];
	strtol (value.c_str(), spos, 0);
	if (*spos == value.c_str()) {
		fail = true;
		return 0;
		}
	return stol (value);
}

int set_nvidia (barconfig &myconfig, bool json) {
	string out;
	long temp = 0;
	if (json)
		out = myconfig.prefix;
	else
		out = myconfig.line_prefix;
	ifstream infile;
	infile.open("/proc/modules");
	stringstream ss;
	ss << infile.rdbuf();
	string fields, file = ss.str();
	if ( ( file.find ("\nnvidia ") != string::npos ) || ( file.find ("nvidia ") == 0 ) ) {
		bool fail;
		fail = false;
		char buffer[128];
		FILE *fd = popen (myconfig.program.c_str(), "r");
		if (fd == NULL)
			return 0;
		while (fgets (buffer, 128, fd) != NULL)
			fields.append(buffer);
		if (fields.size() > 0)
			temp = string_to_long (fields.substr (fields.find (myconfig.format) + myconfig.offset, 2), fail);
		else
			fail = true;
		if (fail == true)
			out += "NA";
		else
			out += to_string (temp) + "°C";
		if (json) {
			json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
			if (myconfig.name.size() > 0)
				json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
			if (temp >= myconfig.urgent) {
				json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent.c_str()));
				json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent.c_str()));
			} else {
				json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
				json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color.c_str()));
				}
			set_icon (myconfig);
		} else
			myconfig.line_output = out;
		pclose(fd);
		}
	return 0;
}
