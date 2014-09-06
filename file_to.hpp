#ifndef FILE_TO_HPP
#define FILE_TO_HPP

#include <string>

using namespace std;

bool file_to_int (const char *path, int &digit);
bool file_to_long (const char *path, long &digit);
bool file_to_string (const char *path, string &line);

#endif
