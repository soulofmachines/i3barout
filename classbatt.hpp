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
    capacity;
    float    powerNow,
    energyNow,
    energyFull,
    voltageNow;
    bool pluggable;
    long seconds;
    tm* timeinfo;
public:
    classBatt();
    void readCustomConfig(yajl_val &config);
    void update();
    bool battCapacity();
    bool battStatus();
    bool battTime();
};

#endif //CLASSHWMON_HPP
