#include <iostream>
#include <vector>
#include <json-c/json.h>
#include <string.h>

using namespace std;

void get_input (vector <string> input_name, vector <string> input_exec) {
	string input, input_ex, name;
	size_t begin, end;
	json_object *jobj;
	json_tokener_error jerr;
	while (true) {
		getline(cin, input);
		begin = input.find_first_of("{");
		end = input.find_first_of("}");
		if ((begin != string::npos) && (end != string::npos)) {
			input_ex = input.substr (begin, end - begin + 1);
			jobj = json_tokener_parse_verbose (input_ex.c_str(), &jerr);
			if (jerr == json_tokener_success) {
				json_object_object_foreach(jobj, key, val) {
					if (strcmp (key, "name") == 0) {
						for (int counter = 0; counter < input_name.size(); ++counter) {
							if (strcmp (json_object_get_string(val), input_name[counter].c_str()) == 0) {
								if (input_exec[counter].size() != 0)
									system (input_exec[counter].c_str());
								}
							}
						}
					}
				}
			}
		}
	return;
	}
