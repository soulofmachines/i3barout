#ifndef CLASSHWMON_HPP
#define CLASSHWMON_HPP

#include "classbase.hpp"

class classHwmon : public classBase {
private:
    std::string monitor;
    int         divider,
    ok;
public:
    classHwmon();
    void readCustomConfig(Json::Value &config);
    void update();
};

#endif //CLASSHWMON_HPP
