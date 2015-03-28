#ifndef FILE_HPP
#define FILE_HPP

#include <string>

int fileToInt(std::string path);
float fileToFloat(std::string path);
std::string fileToString(std::string path);
bool fileExist(std::string path);

#endif //FILE_HPP
