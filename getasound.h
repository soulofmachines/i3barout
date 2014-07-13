#include <alsa/asoundlib.h>

#ifndef GETASOUND_H_

using namespace std;

int getasound (barconfig &myconfig) {
string out, icon;
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
out = to_string (volume) + "%";
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string (out.c_str()));
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color));
if (myconfig.icon != NULL) {
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
} else 
if (myconfig.icon_mask != NULL)
for (int counter = myconfig.icon_count; counter >= 0; --counter) {
if (volume <= 100 * counter / myconfig.icon_count)
icon = myconfig.icon_mask + to_string (counter) + myconfig.icon_ext;
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (icon.c_str()));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
if (volume > 100) {
icon = myconfig.icon_mask + to_string (myconfig.icon_count) + myconfig.icon_ext;
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (icon.c_str()));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color));
}
}
} else {
json_object_object_add(myconfig.json_output, "full_text", json_object_new_string ("0%"));
json_object_object_add(myconfig.json_output, "color", json_object_new_string (myconfig.color_warn));
if (myconfig.icon != NULL) {
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (myconfig.icon));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_warn));
} else
if (myconfig.icon_mask != NULL) {
icon = myconfig.icon_mask + to_string (0) + myconfig.icon_ext;
json_object_object_add(myconfig.json_output, "icon", json_object_new_string (icon.c_str()));
json_object_object_add(myconfig.json_output, "icon_color", json_object_new_string (myconfig.color_warn));
}
}
snd_mixer_close(handle);
return 0;
}

#endif
