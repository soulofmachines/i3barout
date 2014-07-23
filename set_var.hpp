#ifndef SET_VAR_HPP
#define SET_VAR_HPP

int set_asound (barconfig &myconfig);
int set_battery (barconfig &myconfig);
int set_hwmon (barconfig &myconfig);
int set_ip4 (barconfig &myconfig);
int set_nvidia (barconfig &myconfig);
int set_time (barconfig &myconfig);
int set_wlan (barconfig &myconfig);
int set_pause (int value);

#endif
