#ifndef JSON_HPP
#define JSON_HPP

#include <yajl/yajl_tree.h>
#include <yajl/yajl_gen.h>
#include <string>

bool jsonGetBool(yajl_val &config, std::string name, bool def);
int jsonGetInt(yajl_val &config, std::string name, int def);
std::string jsonGetString(yajl_val &config, std::string name, std::string def);
void jsonMapAddString(yajl_gen &json, std::string name, std::string value);

#endif //JSON_HPP
