#include <time.h>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

int set_time (barconfig &myconfig, bool json) {
	string out;
	if (json)
		out = myconfig.prefix;
	else
		out = myconfig.line_prefix;
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[128];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer, 128, myconfig.format.c_str(), timeinfo);
	out += buffer;
	if (json) {
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
		if (myconfig.name.size() > 0)
			json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
		set_icon (myconfig);
	} else
		myconfig.line_output = out;
	return 0;
}
