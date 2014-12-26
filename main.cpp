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
#include "string_to.hpp"

#include <fstream>
#include <getopt.h>
#include <jsoncpp/json/json.h>
#include <math.h>
#include <vector>
#include <future>

using namespace std;

Json::Value j_output;
string s_output = "";
bool b_json = true;
bool b_loop = true;
bool b_input = true;
bool b_colored = true;
bool b_tmux = false;
int timeout = 5;
string color_normal = "#ffffff", color_urgent = "#ff0000", color_warning = "#00ffff";
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
    if (b_colored) {
        if (my_bar_config.color.empty()) {
            if (my_bar_config.integer > -1) {
                if (my_bar_config.integer > my_bar_config.urgent)
                    my_bar_config.color = my_bar_config.color_urgent;
                else
                    my_bar_config.color = my_bar_config.color_normal;
            } else {
                my_bar_config.color = my_bar_config.color_warning;
            }
        }
    }
    switch (b_json) {
    case true:
        output["name"] = my_bar_config.name;
        output["full_text"] = my_bar_config.label + my_bar_config.output;
        output["color"] = my_bar_config.color;
        output["icon_color"] = my_bar_config.color;
        if (!my_bar_config.icon_name.empty()) {
            if (!my_bar_config.icon_mask.empty()) {
                if (my_bar_config.integer > -1) {
                    if (my_bar_config.integer < 100) {
                        my_bar_config.icon = my_bar_config.icon_mask + to_string((int)ceil((float)my_bar_config.integer*(float)my_bar_config.icon_count/(float)100)) + my_bar_config.icon_ext;
                    } else {
                        my_bar_config.icon = my_bar_config.icon_mask + to_string(my_bar_config.icon_count) + my_bar_config.icon_ext;
                    }
                } else {
                    my_bar_config.icon = my_bar_config.icon_mask + "0" + my_bar_config.icon_ext;
                }
                output["icon"] = my_bar_config.icon;
            } else {
                my_bar_config.icon = my_bar_config.icon_name;
                output["icon"] = my_bar_config.icon;
            }
        }
        j_output.append (output);
        break;
    case false:
        if (!s_output.empty())
            s_output += " | ";
        if (b_colored) {
            if (b_tmux) {
                s_output += "#[fg=" + my_bar_config.color + "]" + my_bar_config.label + my_bar_config.output + "#[fg=" + color_normal + "]";
            } else {
                s_output += "\033[" + my_bar_config.color + "m" + my_bar_config.label + my_bar_config.output + "\033[" + color_normal + "m";
            }
        } else {
            s_output += my_bar_config.label + my_bar_config.output;
        }
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
    switch (b_json) {
    case true:
        if (j_output.empty())
            cout << endl;
        else
            cout << writer.write (j_output) << "," << endl;
        j_output.clear();
        break;
    case false:
        if (s_output.empty())
            cout << endl;
        else
            cout << s_output << endl;
        s_output.clear();
        break;
    }
}

void show_help (string cmd) {
    cout << "usage: " << cmd << " config.json" << endl;
}

int read_config (string config_path) {
    Json::Value config;
    Json::Value element;
    Json::Reader reader;
    ifstream config_file;
    config_file.open (config_path);
    if (config_path.empty()) {
        cerr << "no config file" << endl;
        return -1;
    }
    if (!config_file.is_open()) {
        cerr << config_path << ": can't open" << endl;
        return -1;
    }
    if (!reader.parse (config_file, config, false)) {
        cerr << config_path << ": " << reader.getFormattedErrorMessages() << endl;
        return 1;
    }
    config_file.close();
    b_json = config.get("json", true).asBool();
    b_loop = config.get("loop", true).asBool();
    b_input = config.get("input", true).asBool();
    b_colored = config.get("colored", true).asBool();
    b_tmux = config.get("tmux", false).asBool();
    timeout = config.get("timeout", 5).asInt();
    if (b_colored) {
        if (b_json) {
            color_normal = config.get ("color_normal", "#ffffff").asString();
            color_urgent = config.get ("color_urgent", "#ff0000").asString();
            color_warning = config.get ("color_warning", "#00ffff").asString();
        } else {
            if (b_tmux) {
                color_normal = config.get("color_normal", "white").asString();
                color_urgent = config.get("color_urgent", "red").asString();
                color_warning = config.get("color_warning", "cyan").asString();
            } else {
                color_normal = config.get("color_normal", "37").asString();
                color_urgent = config.get("color_urgent", "31").asString();
                color_warning = config.get("color_warning", "36").asString();
            }
        }
    }
    if (!b_json || !b_loop)
        b_input = false;
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
            my_bar_config.back().label = element.get ("label", "").asString();
            if (b_colored) {
                my_bar_config.back().urgent = element.get ("urgent", 0).asInt();
                my_bar_config.back().color_normal = element.get ("color_normal", color_normal).asString();
                my_bar_config.back().color_urgent = element.get ("color_urgent", color_urgent).asString();
                my_bar_config.back().color_warning = element.get ("color_warning", color_warning).asString();
            }
            if (b_json) {
                my_bar_config.back().align = element.get ("align", "center").asString();
                my_bar_config.back().icon_name = element.get ("icon", "").asString();
                my_bar_config.back().width = element.get ("width", false).asBool();
                if (!my_bar_config.back().icon_name.empty()) {
                    size_t begin = my_bar_config.back().icon_name.find_first_of("%");
                    size_t end = my_bar_config.back().icon_name.find_last_of(".");
                    if ((begin != string::npos) && (end != string::npos)) {
                        my_bar_config.back().icon_mask = (my_bar_config.back().icon_name.substr(0,begin));
                        my_bar_config.back().icon_ext = (my_bar_config.back().icon_name.substr(end));
                        if (!string_to_int (my_bar_config.back().icon_name.substr(begin+1,end-begin-1), my_bar_config.back().icon_count)) {
                            cerr << my_bar_config.back().name << ": icon format error" << endl;
                            return 1;
                        }
                    }
                }
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
        while ((cmd = getopt (argc, argv, "?")) != -1) {
            switch (cmd) {
            case '?':
                show_help(argv[0]);
                return 1;
                break;
            }
        }
    } else {
        show_help(argv[0]);
        return 1;
    }
    if (optind < argc) {
        while (optind < argc) {
            config_file = argv[optind++];
        }
    } else {
        show_help(argv[0]);
        return 1;
    }
    if (read_config (config_file) != 0)
        return 1;
    if (my_bar_config.empty())
        return 1;
    future <void> input = async (launch::async, get_input, my_input_config);
    if (b_json) {
        if (b_input)
            cout << "{ \"version\": 1, \"click_events\": true }" << endl;
        else
            cout << "{ \"version\": 1 }" << endl;
        cout << "[" << endl;
    }
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
                if (b_colored)
                    my_bar_config.at (counter).color = my_bar_config.at (counter).color_urgent;
                add_output (my_bar_config.at (counter));
            }
        }
        show_output();
        if (b_loop)
            set_pause (timeout);
    } while (b_loop);
    return 0;
}
