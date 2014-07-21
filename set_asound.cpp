#include <alsa/asoundlib.h>
#include "barconfig.hpp"
#include "set_icon.hpp"

using namespace std;

int set_asound (barconfig &myconfig) {
	string out = myconfig.prefix, icon, width = myconfig.prefix;
	width += "100%";
	int volume_mute, volume;
	long volume_cur, volume_min, volume_max;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *selem_id;
	const snd_mixer_selem_channel_id_t selem_channel = SND_MIXER_SCHN_FRONT_LEFT;
	snd_mixer_open (&handle, 0);
	snd_mixer_attach (handle, myconfig.card);
	snd_mixer_selem_register (handle, NULL, NULL);
	snd_mixer_load (handle);
	snd_mixer_selem_id_alloca (&selem_id);
	snd_mixer_selem_id_set_index (selem_id, 0);
	snd_mixer_selem_id_set_name (selem_id, myconfig.device);
	snd_mixer_elem_t *elem = snd_mixer_find_selem (handle, selem_id);
	snd_mixer_selem_get_playback_switch (elem, selem_channel, &volume_mute);
	if (volume_mute == 1) {
		snd_mixer_selem_get_playback_volume(elem, selem_channel, &volume_cur);
		snd_mixer_selem_get_playback_volume_range(elem, &volume_min, &volume_max);
		volume = int (volume_cur * 100 / (volume_max - volume_min));
		out += to_string (volume) + "%";
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
		json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name));
		if (myconfig.width)
			json_object_object_add(myconfig.json_output, "min_width", json_object_new_string (width.c_str()));
		json_object_object_add(myconfig.json_output, "align", json_object_new_string (myconfig.align));
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
		set_icon (myconfig);
		set_icon_mask (myconfig, volume, 100);
	} else {
		out += "0%";
		json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
		json_object_object_add(myconfig.json_output, "name", json_object_new_string (myconfig.name));
		if (myconfig.width)
			json_object_object_add(myconfig.json_output, "min_width", json_object_new_string (width.c_str()));
		json_object_object_add(myconfig.json_output, "align", json_object_new_string (myconfig.align));
		json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_urgent));
		json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_urgent));
		set_icon (myconfig);
		set_icon_mask_zero (myconfig);
		}
	snd_mixer_close(handle);
	return 0;
	}
