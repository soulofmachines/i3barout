#include <fstream>
#include <sstream>

using namespace std;

long file_to_long (const char *path, bool &fail) {
	char *spos[1];
	ifstream infile;
	stringstream ss;
	infile.open(path);
	if (!infile.is_open()) {
		fail = true;
		return 0;
		}
	ss << infile.rdbuf();
	infile.close();
	string file = ss.str();
	strtol (file.c_str(), spos, 0);
	if (*spos == file.c_str()) {
		fail = true;
		return 0;
		}
	return stol (file);
	}

string file_to_string (const char *path, bool &fail) {
	ifstream infile;
	stringstream ss;
	infile.open (path);
	if (!infile.is_open()) {
		fail = true;
		return "";
		}
	ss << infile.rdbuf();
	infile.close();
	return ss.str();
	}
