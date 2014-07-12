#include <fstream>
#include <sstream>
#include <time.h>

#ifndef GETHWMON_H_

using namespace std;

bool sec_fail = 0;

int filetolong (const char *path, long &value) {
char *spos[1];
ifstream infile;
stringstream ss;
infile.open(path);
if (!infile.is_open()) {
sec_fail = 1;
return 0;
}
ss << infile.rdbuf();
string file = ss.str();
strtol (file.c_str(), spos, 0);
if (*spos == file.c_str()) {
sec_fail = 1;
return 0;
}
value = stol (file);
return 0;
}

int getbat (barconfig &myconfig) {
string out, path;
long perc, e_now, e_full ,p_now, seconds;
path = myconfig.device;
path += "/status";
ifstream infile;
stringstream ss;
string file;
infile.open (path.c_str());
if (!infile.is_open())
return 0;
ss << infile.rdbuf();
file = ss.str();
infile.close();
if (strcmp (file.c_str(), "Discharging\n") == 0) {
path = myconfig.device;
path += "/energy_now";
filetolong (path.c_str(), e_now);
path = myconfig.device;
path += "/power_now";
filetolong (path.c_str(), p_now);
seconds = e_now * 3600 / p_now;
} else
if (strcmp (file.c_str(), "Charging\n") == 0) {
path = myconfig.device;
path += "/energy_full";
filetolong (path.c_str(), e_full);
path = myconfig.device;
path += "/energy_now";
filetolong (path.c_str(), e_now);
path = myconfig.device;
path += "/power_now";
filetolong (path.c_str(), p_now);
if (p_now != 0)
seconds = (e_full - e_now) * 3600 / p_now;
else
seconds = 0;
} else
return 0;
if (sec_fail == 1)
return 0;
path = myconfig.device;
path += "/capacity";
filetolong (path.c_str(), perc);
time_t tseconds(seconds);
tm *tpoint = gmtime(&seconds);
char buffer[128];
strftime (buffer,128,"%H:%M",tpoint);
out = buffer;
out += " " + to_string (perc) + "%";
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
return 0;
}

#endif
