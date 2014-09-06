#include "set_ipv4.hpp"
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>

int set_ipv4 (bar_config &my_bar_config) {
    int fd, return_value = 99;
    struct ifreq ifr;
    fd = socket (AF_INET, SOCK_DGRAM, 0);
    strncpy (ifr.ifr_name, my_bar_config.device.c_str(), IFNAMSIZ-1);
    ioctl (fd, SIOCGIFADDR, &ifr);
    close (fd);
    my_bar_config.output = inet_ntoa (((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr);
    if (my_bar_config.output == "0.0.0.0") {
        return_value = -1;
        goto close;
    }
    return_value = 0;
close:
    return return_value;
}
