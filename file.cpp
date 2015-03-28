#include <fstream>
#include <sstream>
#include "file.hpp"
#include "string.hpp"
#include "exception.hpp"

int fileToInt(std::string path) {
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        throw errorExc("open");
    }
    ss << fileStream.rdbuf();
    fileStream.close();
    try {
        return stringToInt(ss.str());
    }
    catch(...) {
        throw errorExc("convert");
        return 0;
    }
}

float fileToFloat(std::string path) {
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        throw errorExc("open");
    }
    ss << fileStream.rdbuf();
    fileStream.close();
    try {
        return stringToFloat(ss.str());
    }
    catch(...) {
        throw errorExc("convert");
        return 0;
    }
}

std::string fileToString(std::string path) {
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        throw errorExc("open");
    }
    ss << fileStream.rdbuf();
    fileStream.close();
    return ss.str();
}

bool fileExist(std::string path) {
    std::ifstream fileStream;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        return false;
    }
    fileStream.close();
    return true;
}
