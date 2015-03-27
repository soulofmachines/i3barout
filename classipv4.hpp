#ifndef CLASSIPV4_HPP
#define CLASSIPV4_HPP

#include <net/if.h>
#define _LINUX_IF_H
#include "classbase.hpp"

class classIpv4 : public classBase {
private:
    int             fd,
    paddedInt;
    struct ifreq    ifr;
    std::string     device,
    paddedTemp,
    paddedOutput;
    unsigned int padding,
    paddedLen;
public:
    classIpv4();
    void readCustomConfig(yajl_val &config);
    void update();
};

#endif //CLASSIPV4_HPP
