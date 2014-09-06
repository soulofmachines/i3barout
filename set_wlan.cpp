#include "set_wlan.hpp"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int set_wlan (bar_config &my_bar_config) {
    int return_value = 99;
    struct iwreq req;
    struct iw_statistics stat;
    struct iw_range range;
    int soketfd;
    char *id[IW_ESSID_MAX_SIZE+1];
    memset(&req, 0, sizeof(iwreq));
    req.u.essid.length = IW_ESSID_MAX_SIZE+1;
    memcpy(req.ifr_name, my_bar_config.device.c_str(), my_bar_config.device.size());
    if((soketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        return_value = 1;
        goto close;
    }
    req.u.essid.pointer = id;
    if (ioctl(soketfd,SIOCGIWESSID, &req) == -1) {
        return_value = 2;
        goto close;
    }
    my_bar_config.output = (char *)req.u.essid.pointer;
    memset(&stat, 0, sizeof(stat));
    req.u.data.pointer = &stat;
    req.u.data.length = sizeof(stat);
    if(ioctl(soketfd, SIOCGIWSTATS, &req) == -1) {
        my_bar_config.output = "Null";
        return_value = 0;
        goto close;
    }
    memset(&range, 0, sizeof(range));
    req.u.data.pointer = &range;
    req.u.data.length = sizeof(range);
    if(ioctl(soketfd, SIOCGIWRANGE, &req) == -1) {
        return_value = 3;
        goto close;
    }
    my_bar_config.integer = int (char (stat.qual.qual)) * 100 / int (char (range.max_qual.qual));
    my_bar_config.output += " " + to_string (my_bar_config.integer) + "%";
close:
    close(soketfd);
    return return_value;
}
