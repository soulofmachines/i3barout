#ifndef JSONGET_HPP
#define JSONGET_HPP

#include <yajl/yajl_tree.h>
#include <string>

bool jsonGetBool(yajl_val &config, std::string name, bool def);
int jsonGetInt(yajl_val &config, std::string name, int def);
std::string jsonGetString(yajl_val &config, std::string name, std::string def);

#endif //JSONGET_HPP
