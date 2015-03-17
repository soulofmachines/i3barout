#ifndef FILE_HPP
#define FILE_HPP

#include <string>

bool fileToInt(std::string path, int &output, int &ok);
bool fileToFloat(std::string path, float &output, int &ok);
bool fileToString(std::string path, std::string &output, int &ok);
std::string fileToIntError(int ok);
std::string fileToStringError(int ok);
bool fileExist(std::string path);

#endif //FILE_HPP
