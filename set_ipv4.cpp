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
    strncpy (ifr.ifr_name, my_bar_config.input.device.c_str(), IFNAMSIZ-1);
    ioctl (fd, SIOCGIFADDR, &ifr);
    if (ioctl (fd, SIOCGIFFLAGS, &ifr) != -1) {
        if ((ifr.ifr_flags&IFF_RUNNING) != IFF_RUNNING) {
            return_value = 1;
            goto close;
        }
    } else {
        return_value = 2;
        goto close;
    }
    my_bar_config.output.integer = 0;
    my_bar_config.output.output = inet_ntoa (((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr);
    return_value = 0;
close:
    close (fd);
    return return_value;
}
