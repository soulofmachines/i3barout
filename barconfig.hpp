#include <string>
#include <json-c/json.h>

#ifndef BARCONFIG_HPP
#define BARCONFIG_HPP

using namespace std;

enum modes {
	m_null = 0,
	m_asound,
	m_battery,
	m_hwmon,
	m_nvidia,
	m_time,
	m_wlan
	};

class barconfig {
	public:
		modes mode;
		json_object *json_output;
		char *align, *color, *color_urgent;
		char *icon, *icon_mask, *icon_ext;
		int icon_count;
		const char *card, *device, *format, *program;
		int offset, urgent;
		char *name;
		bool width;
	};


#endif
