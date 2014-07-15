#include <time.h>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

int set_time (barconfig &myconfig) {
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[128];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer, 128, myconfig.format, timeinfo);
	json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (buffer));
	json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name));
	json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
	set_icon (myconfig);
	return 0;
}
