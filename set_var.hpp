#ifndef SET_VAR_HPP
#define SET_VAR_HPP

int set_asound (barconfig &myconfig, bool line);
int set_battery (barconfig &myconfig, bool line);
int set_hwmon (barconfig &myconfig, bool line);
int set_ip4 (barconfig &myconfig, bool line);
int set_nvidia (barconfig &myconfig, bool line);
int set_time (barconfig &myconfig, bool line);
int set_wlan (barconfig &myconfig, bool line);
int set_pause (int value);

#endif
