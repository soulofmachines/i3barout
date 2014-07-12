#include <fstream>
#include <sstream>

#ifndef GETHWMON_H_

using namespace std;

int gethwmon (barconfig &myconfig) {
myconfig.json_output = json_object_new_object();
string out;
ifstream infile;
infile.open (myconfig.device);
stringstream ss;
ss << infile.rdbuf();
string file = ss.str();
if (file.size() == 0)
return 0;
out = to_string (stoi (file) / myconfig.offset) + "°C";
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
return 0;
}

#endif
