#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include "classwlan.hpp"
#include "jsonget.hpp"

classWlan::classWlan () {
}

void classWlan::readConfig(Json::Value &config) {
    device = jsonGetString(config["device"], "wlan0");
    memset(&iwr, 0, sizeof(iwreq));
    memset(&iwstat, 0, sizeof(iw_statistics));
    memset(&iwrange, 0, sizeof(iw_range));
    strncpy(iwr.ifr_name, device.c_str(), IFNAMSIZ - 1);
    id = new char[IW_ESSID_MAX_SIZE + 1];
    iwr.u.essid.pointer = id;
}

void classWlan::update() {
    integer = 0;
    output.clear();
    error.clear();
    if (!wlanName())
        return;
    if (!wlanStrength())
        return;
    output = wname + " " + std::to_string(integer) + "%";
}

bool classWlan::wlanName() {
    bool ret = false;
    wname.clear();
    iwr.u.essid.length = IW_ESSID_MAX_SIZE + 1;
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        error = "Wlan: open";
        return ret;
    }
    if (ioctl(fd, SIOCGIWESSID, &iwr) == -1) {
        error = "Wlan: device";
        goto end;
    }
    wname = (char*)iwr.u.essid.pointer;
    ret = true;
end:
    close(fd);
    return ret;
}

bool classWlan::wlanStrength() {
    bool ret = false;
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        error = "Wlan: open";
        return ret;
    }
    iwr.u.data.pointer = &iwrange;
    iwr.u.data.length = sizeof(iw_range);
    if(ioctl(fd, SIOCGIWRANGE, &iwr) == -1) {
        error = "Wlan: device";
        goto end;
    }
    iwr.u.data.pointer = &iwstat;
    iwr.u.data.length = sizeof(iw_statistics);
    if(ioctl(fd, SIOCGIWSTATS, &iwr) == -1) {
        error = "Wlan: status";
        goto end;
    }
    integer = int(char(iwstat.qual.qual)) * 100 / int(char(iwrange.max_qual.qual));
    ret = true;
end:
    close(fd);
    return ret;
}
