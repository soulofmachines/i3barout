#ifndef SET_VAR_H
#define SET_VAR_H

int set_asound (barconfig &myconfig);
int set_battery (barconfig &myconfig);
int set_hwmon (barconfig &myconfig);
int set_nvidia (barconfig &myconfig);
int set_time (barconfig &myconfig);
int set_wlan (barconfig &myconfig);
int set_pause (int value);

#endif
