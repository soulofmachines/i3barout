#include "file_to.hpp"
#include <fstream>
#include <sstream>

using namespace std;

bool file_to_int (const char *path, int &digit) {
    char *spos[1];
    ifstream infile;
    stringstream ss;
    infile.open(path);
    if (!infile.is_open())
        return false;
    ss << infile.rdbuf();
    infile.close();
    string file = ss.str();
    strtol (file.c_str(), spos, 0);
    if (*spos == file.c_str())
        return false;
    digit = stoi (file);
    return true;
}

bool file_to_long (const char *path, long &digit) {
    char *spos[1];
    ifstream infile;
    stringstream ss;
    infile.open(path);
    if (!infile.is_open())
        return false;
    ss << infile.rdbuf();
    infile.close();
    string file = ss.str();
    strtol (file.c_str(), spos, 0);
    if (*spos == file.c_str())
        return false;
    digit = stol (file);
    return true;
}

bool file_to_string (const char *path, string &line) {
    ifstream infile;
    stringstream ss;
    infile.open (path);
    if (!infile.is_open())
        return false;
    ss << infile.rdbuf();
    infile.close();
    line = ss.str();
    return true;
}
