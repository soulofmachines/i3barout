#include "main.hpp"
#include "get_input.hpp"
#include "set_asound.hpp"
#include "set_battery.hpp"
#include "set_hwmon.hpp"
#include "set_ipv4.hpp"
#include "set_nvidia.hpp"
#include "set_pause.hpp"
#include "set_time.hpp"
#include "set_wlan.hpp"

#include <fstream>
#include <getopt.h>
#include <jsoncpp/json/json.h>
#include <vector>
#include <future>

using namespace std;

Json::Value j_output;
string s_output = "";
views e_view = v_json;
bool b_infinite = true;
bool b_pause = true;
bool b_input = true;
vector <bar_config> my_bar_config;
vector <input_config> my_input_config;

modes string2modes (string input) {
    if (input == "asound") return m_asound;
    if (input == "battery") return m_battery;
    if (input == "hwmon") return m_hwmon;
    if (input == "ipv4") return m_ipv4;
    if (input == "nvidia") return m_nvidia;
    if (input == "time") return m_time;
    if (input == "wlan") return m_wlan;
    return m_null;
}

void add_output (bar_config my_bar_config) {
    Json::Value output;
    switch (e_view) {
    case v_json:
        output["name"] = my_bar_config.name;
        output["full_text"] = my_bar_config.prefix + my_bar_config.output;
        if (my_bar_config.integer > -1) {
            if (my_bar_config.integer > my_bar_config.urgent)
                my_bar_config.color = my_bar_config.color_urgent;
            else
                my_bar_config.color = my_bar_config.color_normal;
        } else {
            my_bar_config.color = my_bar_config.color_warning;
        }
        output["color"] = my_bar_config.color;
        output["icon_color"] = my_bar_config.color;
        j_output.append (output);
        break;
    case v_line:
        if (!s_output.empty())
            s_output += " | ";
        s_output += my_bar_config.prefix + my_bar_config.output;
        break;
    }
    my_bar_config.output.clear();
    my_bar_config.max_output.clear();
    my_bar_config.icon.clear();
    my_bar_config.color.clear();
    my_bar_config.integer = -1;
}

void show_output () {
    Json::FastWriter writer;
    switch (e_view) {
    case v_json:
        if (j_output.empty())
            cout << endl;
        else
            cout << writer.write (j_output);
        j_output.clear();
        break;
    case v_line:
        if (s_output.empty())
            cout << endl;
        else
            cout << s_output << endl;
        s_output.clear();
        break;
    }
}

int read_config (string config_path) {
    Json::Value config;
    Json::Value element;
    Json::Reader reader;
    ifstream config_file;
    config_file.open (config_path);
    if (!config_file.is_open()) {
        cerr << config_path << ": can't open" << endl;
        return -1;
    }
    if (!reader.parse (config_file, config, false)) {
        cerr << config_path << ": " << reader.getFormattedErrorMessages() << endl;
        return 1;
    }
    config_file.close();
    vector <string> names = config.getMemberNames();
    for (unsigned int counter = 0; counter < names.size(); ++counter) {
        if (config[names[counter]].isObject()) {
            my_bar_config.push_back (bar_config());
            element = config[names.at (counter)];
            my_bar_config.back().name = names.at (counter);
            my_bar_config.back().mode = string2modes (element.get ("mode", "").asString());
            my_bar_config.back().device = element.get ("device", "").asString();
            my_bar_config.back().offset = element.get ("offset", 0).asInt();
            my_bar_config.back().param = element.get ("param", "").asString();
            my_bar_config.back().prefix = element.get ("prefix", "").asString();
            if (e_view == v_json) {
                my_bar_config.back().align = element.get ("align", "center").asString();
                my_bar_config.back().color_normal = element.get ("color_normal", "#ffffff").asString();
                my_bar_config.back().color_urgent = element.get ("color_urgent", "#ff0000").asString();
                my_bar_config.back().color_warning = element.get ("color_warning", "#00ffff").asString();
                my_bar_config.back().icon_count = element.get ("icon_count", 0).asInt();
                my_bar_config.back().icon_ext = element.get ("icon_ext", "xbm").asString();
                my_bar_config.back().icon_mask = element.get ("icon_mask", "").asString();
                my_bar_config.back().icon_name = element.get ("icon_name", "").asString();
                my_bar_config.back().urgent = element.get ("urgent", 0).asInt();
                my_bar_config.back().width = element.get ("width", false).asBool();
            }
            if (my_bar_config.back().mode == m_null)
                my_bar_config.pop_back();
            else
                if (b_input) {
                    my_input_config.push_back (input_config());
                    my_input_config.back().name = names.at (counter);
                    my_input_config.back().exec1 = element.get ("exec1", "").asString();
                    my_input_config.back().exec2 = element.get ("exec2", "").asString();
                    my_input_config.back().exec3 = element.get ("exec3", "").asString();
                    if ((my_input_config.back().exec1.size() == 0) && (my_input_config.back().exec2.size() == 0) && (my_input_config.back().exec3.size() == 0))
                        my_input_config.pop_back();
                }
        }
    }
    return 0;
}

int main (int argc, char *argv[]) {
    int return_value;
    string config_file = "";
    if (argc > 1) {
        int cmd;
        while ((cmd = getopt (argc, argv, "1c:il")) != -1) {
            switch (cmd) {
            case '1':
                b_infinite = false;
                b_pause = false;
                b_input = false;
                break;
            case 'c':
                config_file = optarg;
                break;
            case 'i':
                b_pause = false;
                break;
            case 'l':
                e_view = v_line;
                b_input = false;
                break;
            }
        }
    }
    if (config_file.size() != 0)
        read_config (config_file);
    if (my_bar_config.size() < 1)
        read_config ("config.json");
    if (my_bar_config.size() < 1)
        return 1;
    future <void> input = async (launch::async, get_input, my_input_config);
    do {
        return_value = 0;
        for (unsigned int counter = 0; counter < my_bar_config.size(); ++counter) {
            switch (my_bar_config.at (counter).mode) {
            case m_null:
                break;
            case m_asound:
                return_value = set_asound (my_bar_config.at (counter));
                break;
            case m_battery:
                return_value = set_battery (my_bar_config.at (counter));
                break;
            case m_hwmon:
                return_value = set_hwmon (my_bar_config.at (counter));
                break;
            case m_ipv4:
                return_value = set_ipv4 (my_bar_config.at (counter));
                break;
            case m_nvidia:
                return_value = set_nvidia (my_bar_config.at (counter));
                break;
            case m_time:
                return_value = set_time (my_bar_config.at (counter));
                break;
            case m_wlan:
                return_value = set_wlan (my_bar_config.at (counter));
                break;
            }
            if (return_value == 0) {
                add_output (my_bar_config.at (counter));
            }
            if (return_value > 0) {
                my_bar_config.at (counter).output = "ERR" + to_string (return_value);
                my_bar_config.at (counter).color = "#ff0000";
                add_output (my_bar_config.at (counter));
            }
        }
        show_output();
        if (b_pause)
            set_pause (5);
    } while (b_infinite);
    return 0;
}
