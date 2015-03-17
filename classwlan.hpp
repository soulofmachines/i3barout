#ifndef CLASSWLAN_HPP
#define CLASSWLAN_HPP

#include <linux/wireless.h>
#include "classbase.hpp"

class classWlan : public classBase {
private:
    int                     fd;
    char*                   id;
    struct iwreq            iwr;
    struct iw_statistics    iwstat;
    struct iw_range         iwrange;
    std::string             device;
    std::string             wname;
    bool padded;
public:
    classWlan();
    void readCustomConfig(yajl_val &config);
    void update();
    bool wlanName();
    bool wlanStrength();
};

#endif //CLASSWLAN_HPP
