#include "barconfig.hpp"

using namespace std;

void set_icon (barconfig &myconfig) {
	if (myconfig.icon != NULL)
		json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
	return;
	}

void set_icon_mask (barconfig &myconfig, int value, int max) {
	if (myconfig.icon_mask != NULL) {
		string icon;
		for (int counter = myconfig.icon_count; counter >= 0; --counter)
			if (value <= max * counter / myconfig.icon_count)
				icon = myconfig.icon_mask + to_string (counter) + myconfig.icon_ext;
		if (value > max)
			icon = myconfig.icon_mask + to_string (myconfig.icon_count) + myconfig.icon_ext;
		json_object_object_add(myconfig.json_output, "icon", json_object_new_string (icon.c_str()));
		}
	return;
	}

void set_icon_mask_zero (barconfig &myconfig) {
	if (myconfig.icon_mask != NULL) {
		string icon;
		icon = myconfig.icon_mask + to_string (0) + myconfig.icon_ext;
		json_object_object_add(myconfig.json_output, "icon", json_object_new_string (icon.c_str()));
		}
	return;
	}
