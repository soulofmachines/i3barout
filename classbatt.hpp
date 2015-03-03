#ifndef CLASSBATT_HPP
#define CLASSBATT_HPP

#include "classbase.hpp"
#include <ctime>

class classBatt : public classBase {
private:
    std::string device,
    time,
    status;
    int         ok,
    capacity,
    powerNow,
    energyNow,
    energyFull;
    long seconds;
    tm* timeinfo;
    public:
    classBatt();
    void readCustomConfig(Json::Value &config);
    void update();
    bool battCapacity();
    bool battStatus();
    bool battTime();
};

#endif //CLASSHWMON_HPP
