#include <fstream>
#include <sstream>

#ifndef GETNVIDIA_H_

using namespace std;

int getnvidia (barconfig &myconfig) {
myconfig.json_output = json_object_new_object();
ifstream infile;
infile.open("/proc/modules");
stringstream ss;
ss << infile.rdbuf();
string fields, file = ss.str();
if ( ( file.find ("\nnvidia ") != string::npos ) || ( file.find ("nvidia ") == 0 ) )
{
char buffer[128];
FILE *fd = popen (myconfig.program, "r");
if (fd == NULL)
return 0;
while (fgets (buffer, 128, fd) != NULL)
fields.append(buffer);
int temp = stoi (fields.substr(fields.find(myconfig.format)+myconfig.offset,2));
fields = to_string (temp) + "°C";
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (fields.c_str()));
if (temp >= myconfig.urgent)
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
else
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
pclose(fd);
}
return 0;
}

#endif
