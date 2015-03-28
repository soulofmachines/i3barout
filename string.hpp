#ifndef STRING_HPP
#define STRING_HPP

#include <string>

int stringToInt(std::string input);
float stringToFloat(std::string input);
void stringPadZero(std::string &input, unsigned int size);
unsigned int stringToHash(const char* input, int x = 0);

#endif //STRING_HPP
