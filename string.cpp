#include "string.hpp"
#include "exception.hpp"

int stringToInt(std::string input) {
    try {
        return stoi(input);
    }
    catch (...) {
        throw errorExc("error");
        return 0;
    }
}

float stringToFloat(std::string input) {
    try {
        return stof(input);
    }
    catch (...) {
        throw errorExc("error");
        return 0;
    }
}

void stringPadZero(std::string &input, unsigned int size) {
    if (input.size() < size) {
        input = std::string(size-input.size(), '0') + input;
    }
}

unsigned int stringToHash(const char* input, int x) {
    return !input[x] ? 5381 : (stringToHash(input, x+1)*33) ^ input[x];
}
