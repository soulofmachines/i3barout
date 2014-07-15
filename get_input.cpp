#include <iostream>
#include <vector>

using namespace std;

void get_input (vector <string> input_name, vector <string> input_exec) {
	string input;
	while (true) {
		getline(cin, input);
		for (int counter = 0; counter < input_name.size(); ++counter) {
			size_t found = input.find (input_name[counter]);
			if (found != string::npos)
				if (input_exec[counter].size() != 0)
					system (input_exec[counter].c_str());
			}
		}
	return;
	}
