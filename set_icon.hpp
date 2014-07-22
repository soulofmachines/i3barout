#include <string.h>
#include "barconfig.hpp"

#ifndef SET_ICON_HPP
#define SET_ICON_HPP

using namespace std;

void set_icon (barconfig &myconfig);
void set_icon_mask (barconfig &myconfig, int value, int max);
void set_icon_mask_zero (barconfig &myconfig);

#endif
