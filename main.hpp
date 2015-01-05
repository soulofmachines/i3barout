#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

using namespace std;

enum modes {
    m_null = 0,
    m_asound,
    m_battery,
    m_hwmon,
    m_ipv4,
    m_nvidia,
    m_time,
    m_wlan
};

class bar_config {
public:
    modes mode = m_null;
    //string output = "", icon = "", color = "", max_output = "";
    //string name = "", label = "", param = "", device = "";
    //string color_normal = "#ffffff", color_warning = "#00ffff", color_urgent = "#ff0000", align = "center";
    //string icon_name = "", icon_mask = "", icon_ext = "";
    //int urgent = -1, integer = -1, icon_count = 0, offset = 0;
    //bool width = false;
    class icons {
    public:
        string name = "", mask = "", ext = "";
        int count = 0;
    };
    bar_config::icons icon;
    class outputs {
    public:
        string icon = "", color = "", output = "";
        int integer = -1;
    };
    bar_config::outputs output;
    class colors {
    public:
        string normal = "", warning = "", urgent = "";
    };
    bar_config::colors color;
    class inputs {
    public:
        string name = "", label = "", param = "", device = "";
        int offset = 0, urgent = -1;
    };
    bar_config::inputs input;
    class execs {
    public:
        string exec1 = "", exec2 = "", exec3 = "";
    };
    bar_config::execs exec;
};

/*class input_config {
public:
    string name = "", exec1 = "", exec2 = "", exec3 = "";
};*/

#endif
