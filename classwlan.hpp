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
public:
    classWlan();
    void readConfig(Json::Value &config);
    void update();
    void wlanName();
    void wlanStrength();
};

#endif //CLASSWLAN_HPP
