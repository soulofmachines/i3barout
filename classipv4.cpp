#include "classipv4.hpp"
#include "jsonget.hpp"
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

classIpv4::classIpv4() {
    integer = 0;
    urgent = 0;
}

void classIpv4::readCustomConfig(yajl_val &config) {
    device = jsonGetString(config, "device", "eth0");
    strncpy(ifr.ifr_name, device.c_str(), IFNAMSIZ - 1);
}

void classIpv4::update() {
    output.clear();
    error.clear();
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        error = "Lan: open";
        return;
    }
    if (ioctl(fd, SIOCGIFFLAGS, &ifr) == -1) {
        error = "Lan: device";
        goto end;
    }
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        error = "Lan: status";
        goto end;
    }
    output = inet_ntoa(((struct sockaddr_in*)(&ifr.ifr_addr))->sin_addr);
end:
    close (fd);
}
