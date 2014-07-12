#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#ifndef GETWLAN_H_

using namespace std;

int getwlan (barconfig &myconfig) {
myconfig.json_output = json_object_new_object();
string out;
struct iwreq req;
struct iw_statistics stat;
struct iw_range range;
int soketfd;
char * id;
id = new char[IW_ESSID_MAX_SIZE+1];
memset(&req, 0, sizeof(iwreq));
req.u.essid.length = IW_ESSID_MAX_SIZE+1;
memcpy(req.ifr_name, myconfig.device, strlen (myconfig.device));
if((soketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
return 0;
req.u.essid.pointer = id;
if (ioctl(soketfd,SIOCGIWESSID, &req) == -1)
return 0;
out = (char *)req.u.essid.pointer;
memset(&stat, 0, sizeof(stat));
req.u.data.pointer = &stat;
req.u.data.length = sizeof(stat);
if(ioctl(soketfd, SIOCGIWSTATS, &req) == -1)
return 4;
memset(&range, 0, sizeof(range));
req.u.data.pointer = &range;
req.u.data.length = sizeof(range);
if(ioctl(soketfd, SIOCGIWRANGE, &req) == -1)
return 5;
out = to_string (int (char (stat.qual.qual)) * 100 / (int(char(range.max_qual.qual)))) + "% " + out;
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
if (myconfig.icon != NULL) {
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
}
close(soketfd);
return 0;
}

#endif
