#ifndef CLASSTIME_HPP
#define CLASSTIME_HPP

#include <ctime>
#include "classbase.hpp"

class classTime : public classBase {
private:
    time_t      rawtime;
    tm*         timeinfo;
    int         size;
    std::string format;
public:
    classTime();
    void readConfig(Json::Value &config);
    void update();
};

#endif //CLASSTIME_HPP
