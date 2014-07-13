#include <string>
#include <json-c/json.h>

#ifndef BARCONFIG_HPP
#define BARCONFIG_HPP

using namespace std;

enum modes {
	m_null = 0,
	m_asound,
	m_bat,
	m_hwmon,
	m_nvidia,
	m_time,
	m_wlan
	};

class barconfig {
	public:
		char *color, *color_warn, *color_urgent;
		char *icon, *icon_mask, *icon_ext;
		string name, exec;
		const char *device, *format, *card, *program, *align;
		int offset, urgent, icon_count;
		modes mode;
		json_object *json_output;
	};


#endif
