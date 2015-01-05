#include "get_input.hpp"
#include "main.hpp"
#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

void get_input (vector <bar_config> my_bar_config, bool b_use_input) {
    if (my_bar_config.size() == 0)
        return;
    if (!b_use_input)
        return;
    string input, jinput, name;
    int button;
    size_t begin, end;
    Json::Reader reader;
    Json::Value element;
    while (true) {
        getline(cin, input);
        begin = string::npos;
        end = string::npos;
        name = "";
        button = 0;
        begin = input.find_first_of ("{");
        end = input.find_first_of ("}");
        if ((begin != string::npos) && (end != string::npos)) {
            jinput = input.substr (begin, end - begin + 1);
            if (reader.parse(jinput, element, false)) {
                name = element.get ("name", "").asString();
                button = element.get ("button", "0").asInt();
                if ((name.size() != 0) && (button != 0))
                    for (unsigned int counter = 0; counter < my_bar_config.size(); ++counter)
                        if (my_bar_config.at (counter).input.name == name)
                            switch (button) {
                            case 1:
                                if (my_bar_config[counter].exec.exec1.size() != 0)
                                    system (my_bar_config[counter].exec.exec1.c_str());
                                break;
                            case 2:
                                if (my_bar_config[counter].exec.exec2.size() != 0)
                                    system (my_bar_config[counter].exec.exec2.c_str());
                                break;
                            case 3:
                                if (my_bar_config[counter].exec.exec3.size() != 0)
                                    system (my_bar_config[counter].exec.exec3.c_str());
                                break;
                            }
            }
        }
    }
    return;
}
