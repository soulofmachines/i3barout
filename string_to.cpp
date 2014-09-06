#include "string_to.hpp"

using namespace std;

bool string_to_int (string line, int &digit) {
    char *spos[1];
    strtol (line.c_str(), spos, 0);
    if (*spos == line.c_str()) {
        return false;
    }
    digit = stoi (line);
    return true;
}
