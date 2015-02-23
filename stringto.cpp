#include "stringto.hpp"

bool stringToInt(std::string input, int &output) {
    try {
        output = stoi(input);
    }
    catch (...) {
        return false;
    }
    return true;
}
