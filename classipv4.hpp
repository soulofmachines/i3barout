#ifndef CLASSIPV4_HPP
#define CLASSIPV4_HPP

#include <net/if.h>
#define _LINUX_IF_H
#include "classbase.hpp"

class classIpv4 : public classBase {
private:
    int             fd;
    struct ifreq    ifr;
    std::string     device;
public:
    classIpv4();
    void readConfig(Json::Value &config);
    void update();
};

#endif //CLASSIPV4_HPP
