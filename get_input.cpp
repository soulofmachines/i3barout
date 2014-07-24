#include <iostream>
#include <vector>
#include <json-c/json.h>
#include <string.h>
#include "barconfig.hpp"

using namespace std;

void get_input (vector <input_exec> myinput, bool onetime) {
	if (!onetime)
		return;
	string input, jinput, name;
	int button = 0;
	size_t begin, end;
	json_object *jobj;
	json_tokener_error jerr;
	while (true) {
		getline(cin, input);
		begin = input.find_first_of("{");
		end = input.find_first_of("}");
		if ((begin != string::npos) && (end != string::npos)) {
			jinput = input.substr (begin, end - begin + 1);
			jobj = json_tokener_parse_verbose (jinput.c_str(), &jerr);
			if (jerr == json_tokener_success) {
				json_object_object_foreach(jobj, key, val) {
					if (strcmp (key, "name") == 0)
						name = json_object_get_string(val);
					if (strcmp (key, "button") == 0)
						button = json_object_get_int(val);
					}
				for (u_int counter = 0; counter < myinput.size(); ++counter)
					if (myinput[counter].name == name)
						switch (button) {
							case 1:
								if (myinput[counter].exec1.size() != 0)
									system (myinput[counter].exec1.c_str());
								break;
							case 2:
								if (myinput[counter].exec2.size() != 0)
									system (myinput[counter].exec2.c_str());
								break;
							case 3:
								if (myinput[counter].exec3.size() != 0)
									system (myinput[counter].exec3.c_str());
								break;
							}
				}
			}
		}
	return;
	}
