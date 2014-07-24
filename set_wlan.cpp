#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <string.h>
#include <unistd.h>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

int set_wlan (barconfig &myconfig, bool json) {
	string out = myconfig.prefix, name, width = myconfig.prefix;
	if (json) {
		out = myconfig.prefix;
		width = myconfig.prefix;
	} else
		out = myconfig.line_prefix;
	int perc;
	struct iwreq req;
	struct iw_statistics stat;
	struct iw_range range;
	int soketfd;
	char *id[IW_ESSID_MAX_SIZE+1];
	memset(&req, 0, sizeof(iwreq));
	req.u.essid.length = IW_ESSID_MAX_SIZE+1;
	memcpy(req.ifr_name, myconfig.device.c_str(), myconfig.device.size());
	if((soketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return 0;
	req.u.essid.pointer = id;
	if (ioctl(soketfd,SIOCGIWESSID, &req) == -1)
		return 0;
	name = (char *)req.u.essid.pointer;
	memset(&stat, 0, sizeof(stat));
	req.u.data.pointer = &stat;
	req.u.data.length = sizeof(stat);
	if(ioctl(soketfd, SIOCGIWSTATS, &req) == -1) {
		out += "Null";
		if (json) {
			json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
			if (myconfig.name.size() > 0)
				json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
			json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
			set_icon (myconfig);
		} else
			myconfig.line_output = out;
		return 0;
		}
	memset(&range, 0, sizeof(range));
	req.u.data.pointer = &range;
	req.u.data.length = sizeof(range);
	if(ioctl(soketfd, SIOCGIWRANGE, &req) == -1)
		return 0;
	perc = int (char (stat.qual.qual)) * 100 / int (char (range.max_qual.qual));
	close(soketfd);
	out += name + " " + to_string (perc) + "%";
	if (json) {
		width += name + " 100%";
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
		if (myconfig.name.size() > 0)
			json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name.c_str()));
		if (myconfig.width)
			json_object_object_add(myconfig.json_output, "min_width", json_object_new_string (width.c_str()));
		json_object_object_add(myconfig.json_output, "align", json_object_new_string (myconfig.align.c_str()));
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color.c_str()));
		set_icon (myconfig);
	} else
		myconfig.line_output = out;
	return 0;
}
