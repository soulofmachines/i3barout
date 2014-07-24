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
	m_ip4,
	m_nvidia,
	m_time,
	m_wlan
	};

class barconfig {
	public:
		modes mode = m_null;
		json_object *json_output;
		string align = "center", color = "#ffffff", color_urgent = "#ff0000";
		string icon = "", icon_mask = "", icon_ext = "";
		int icon_count = 0;
		string card = "", device = "", format = "", program = "";
		int offset = 0, urgent = 0;
		string name = "", prefix = "", line_output = "";
		bool width = false;
	};

class input_exec {
	public:
		string name = "", exec1 = "", exec2 = "", exec3 = "";
	};

#endif
