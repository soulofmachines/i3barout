#include <fstream>
#include <sstream>

#ifndef FILE_TO_HPP
#define FILE_TO_HPP

using namespace std;

long file_to_long (const char *path, bool &fail);
string file_to_string (const char *path, bool &fail);

#endif
