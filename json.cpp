#include "json.hpp"

bool jsonGetBool(yajl_val &config, std::string name, bool def) {
    yajl_val value;
    const char* path[] = {name.c_str(), (const char*) 0};
    value = yajl_tree_get(config, path, yajl_t_true);
    if (YAJL_IS_TRUE(value))
        return true;
    value = yajl_tree_get(config, path, yajl_t_false);
    if (YAJL_IS_FALSE(value))
        return false;
    return def;
}

int jsonGetInt(yajl_val &config, std::string name, int def) {
    yajl_val value;
    const char* path[] = {name.c_str(), (const char*) 0};
    value = yajl_tree_get(config, path, yajl_t_number);
    if (YAJL_IS_INTEGER(value))
        return YAJL_GET_INTEGER(value);
    return def;
}

std::string jsonGetString(yajl_val &config, std::string name, std::string def) {
    yajl_val value;
    const char* path[] = {name.c_str(), (const char*) 0};
    value = yajl_tree_get(config, path, yajl_t_string);
    if (YAJL_IS_STRING(value))
        return YAJL_GET_STRING(value);
    return def;
}

void jsonMapAddString(yajl_gen &json, std::string name, std::string value) {
    yajl_gen_string(json, (const unsigned char*) name.c_str(), name.length());
    yajl_gen_string(json, (const unsigned char*) value.c_str(), value.length());
}
