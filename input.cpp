#include "input.hpp"
#include <iostream>
#include <yajl/yajl_tree.h>
#include "json.hpp"
#include <unistd.h>

bool inputLoop = true;

void inputProtocol(std::vector<classInput> inputV) {
    if (inputV.size() < 1) {
        return;
    }
    std::string input, name, exec;
    size_t begin, end;
    yajl_val inputStream;
    char errbuf[1024];
    while (inputLoop) {
        name.clear();
        exec.clear();
        begin = std::string::npos;
        end = std::string::npos;
        yajl_tree_free(inputStream);
        std::getline(std::cin, input);
        begin = input.find_first_of ("{");
        end = input.find_first_of ("}");
        if ((begin == std::string::npos) || (end == std::string::npos)) {
            continue;
        } else {
            input = input.substr(begin, end - begin + 1);
        }
        if (input.find("name") == std::string::npos) {
            continue;
        }
        inputStream = yajl_tree_parse(input.c_str(), errbuf, sizeof(errbuf));
        if (inputStream == NULL) {
            continue;
        }
        name = jsonGetString(inputStream, "name", "");
        if (name.empty()) {
            continue;
        }
        for (unsigned int x = 0; x < inputV.size(); ++x) {
            if (name == inputV.at(x).name) {
                switch (jsonGetInt(inputStream, "button", 0)) {
                case 1:
                    exec = inputV.at(x).exec1;
                    break;
                case 2:
                    exec = inputV.at(x).exec2;
                    break;
                case 3:
                    exec = inputV.at(x).exec3;
                    break;
                default:
                    continue;
                    break;
                }
                exec += " 1>/dev/null 2>/dev/null &";
                system(exec.c_str());
            }
        }
    }
}
