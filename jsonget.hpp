#ifndef JSONGET_HPP
#define JSONGET_HPP

#include <jsoncpp/json/json.h>

bool jsonGetBool(Json::Value &config, bool def);
int jsonGetInt(Json::Value &config, int def);
std::string jsonGetString(Json::Value &config, std::string def);

#endif //JSONGET_HPP
