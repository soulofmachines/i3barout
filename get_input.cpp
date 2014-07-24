#include <iostream>
#include <vector>
#include <json-c/json.h>
#include <string.h>

using namespace std;

void get_input (vector <string> input_name, vector <string> input_exec1, vector <string> input_exec2, vector <string> input_exec3, bool onetime) {
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
				for (u_int counter = 0; counter < input_name.size(); ++counter)
					if (input_name[counter] == name)
						switch (button) {
							case 1:
								if (input_exec1[counter].size() != 0)
									system (input_exec1[counter].c_str());
								break;
							case 2:
								if (input_exec2[counter].size() != 0)
									system (input_exec2[counter].c_str());
								break;
							case 3:
								if (input_exec3[counter].size() != 0)
									system (input_exec3[counter].c_str());
								break;
							}
				}
			}
		}
	return;
	}
