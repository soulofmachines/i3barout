#include "jsonget.hpp"

bool jsonGetBool(Json::Value &config, bool def) {
    if (config.isBool())
        return config.asBool();
    else
        return def;
}

int jsonGetInt(Json::Value &config, int def) {
    if (config.isInt())
        return config.asInt();
    else
        return def;
}

std::string jsonGetString(Json::Value &config, std::string def) {
    if (config.isString())
        return config.asString();
    else
        return def;
}
