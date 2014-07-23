#include "barconfig.hpp"
#include "set_icon.hpp"
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>

int set_ip4 (barconfig &myconfig) {
	string out = myconfig.prefix, ip;
	int fd;
	struct ifreq ifr;
	fd = socket (AF_INET, SOCK_DGRAM, 0);
	strncpy (ifr.ifr_name, myconfig.device.c_str(), IFNAMSIZ-1);
	ioctl (fd, SIOCGIFADDR, &ifr);
	close (fd);
	ip = inet_ntoa (((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr);
	if (ip == "0.0.0.0")
		return 0;
	out += ip;
	json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
	json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
	json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
	set_icon (myconfig);
	return 0;
}
