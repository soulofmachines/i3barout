#include <fstream>
#include <sstream>
#include "barconfig.hpp"

using namespace std;

int set_nvidia (barconfig &myconfig) {
	ifstream infile;
	infile.open("/proc/modules");
	stringstream ss;
	ss << infile.rdbuf();
	string stemp, fields, file = ss.str();
	char *spos[1];
	if ( ( file.find ("\nnvidia ") != string::npos ) || ( file.find ("nvidia ") == 0 ) ) {
		char buffer[128];
		FILE *fd = popen (myconfig.program, "r");
		if (fd == NULL)
			return 0;
		while (fgets (buffer, 128, fd) != NULL)
			fields.append(buffer);
		stemp = fields.substr (fields.find (myconfig.format) + myconfig.offset, 2);
		strtol (stemp.c_str(), spos, 0);
		if (*spos == stemp.c_str())
			return 0;
		int temp = stoi (stemp);
		fields = to_string (temp) + "°C";
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (fields.c_str()));
		if (temp >= myconfig.urgent) {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
			if (myconfig.icon != NULL) {
				json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
				json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent));
				}
		} else {
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
			if (myconfig.icon != NULL) {
				json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
				json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
				}
			}
		pclose(fd);
		}
	return 0;
}
