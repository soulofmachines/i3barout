#include "classipv4.hpp"
#include "json.hpp"
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stringto.hpp"

classIpv4::classIpv4() {
    integer = 0;
}

void classIpv4::readCustomConfig(yajl_val &config) {
    device = jsonGetString(config, "device", "eth0");
    padded = jsonGetBool(config, "padded", false);
    padding = jsonGetInt(config, "padding", 3);
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
    if (padded) {
        paddedOutput = "";
        paddedTemp = "";
        paddedLen = 0;
        for (unsigned int x = 0; x < 4; ++x) {
            if (x > 0) {
                paddedOutput += ".";
            }
            if (paddedLen > output.size()) {
                output = "Lan: lenght";
                goto end;
            }
            if (!stringToInt(output.substr(paddedLen), paddedInt)) {
                output = "Lan: padding";
                goto end;
            }
            paddedTemp = std::to_string(paddedInt);
            paddedLen += paddedTemp.size() + 1;
            if (paddedTemp.size() < padding) {
                paddedTemp = std::string(padding-paddedTemp.size(), '0') + paddedTemp;
            }
            paddedOutput += paddedTemp;
        }
        output = paddedOutput;
    }
end:
    close (fd);
}
