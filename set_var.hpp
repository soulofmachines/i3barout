#ifndef SET_VAR_HPP
#define SET_VAR_HPP

int set_asound (barconfig &myconfig, bool json);
int set_battery (barconfig &myconfig, bool json);
int set_hwmon (barconfig &myconfig, bool json);
int set_ipv4 (barconfig &myconfig, bool json);
int set_nvidia (barconfig &myconfig, bool json);
int set_time (barconfig &myconfig, bool json);
int set_wlan (barconfig &myconfig, bool json);
int set_pause (int value);

#endif
