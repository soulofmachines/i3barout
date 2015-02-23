#include <fstream>
#include <sstream>
#include "fileto.hpp"
#include "stringto.hpp"

bool fileToInt(std::string path, int &output, int &ok) {
    ok = -1;
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        ok = -2;
        return false;
    }
    ss << fileStream.rdbuf();
    fileStream.close();
    if (!stringToInt(ss.str(), output)) {
        ok = -3;
        return false;
    }
    ok = 0;
    return true;
}

bool fileToString(std::string path, std::string &output, int &ok) {
    ok = -1;
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.open(path.c_str());
    if (!fileStream.is_open()) {
        ok = -2;
        return false;
    }
    ss << fileStream.rdbuf();
    fileStream.close();
    output = ss.str();
    ok = 0;
    return true;
}