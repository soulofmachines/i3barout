#include "set_asound.hpp"
#include <alsa/asoundlib.h>

using namespace std;

int set_asound (bar_config &my_bar_config) {
    int return_value = 99;
    int volume_unmute, volume;
    long volume_cur, volume_min, volume_max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *selem_id;
    snd_mixer_elem_t *elem;
    const snd_mixer_selem_channel_id_t selem_channel = SND_MIXER_SCHN_FRONT_LEFT;
    if (snd_mixer_open (&handle, 0) < 0) {
        return_value = 1;
        goto close;
    }
    if (snd_mixer_attach (handle, my_bar_config.input.device.c_str()) < 0) {
        return_value = 2;
        goto close;
    }
    if (snd_mixer_selem_register (handle, NULL, NULL) < 0) {
        return_value = 3;
        goto close;
    }
    if (snd_mixer_load (handle) < 0) {
        return_value = 4;
        goto close;
    }
    snd_mixer_selem_id_alloca (&selem_id);
    snd_mixer_selem_id_set_index (selem_id, 0);
    snd_mixer_selem_id_set_name (selem_id, my_bar_config.input.param.c_str());
    elem = snd_mixer_find_selem (handle, selem_id);
    if (!elem) {
        return_value = 5;
        goto close;
    }
    snd_mixer_selem_get_playback_switch (elem, selem_channel, &volume_unmute);
    if (volume_unmute == true) {
        snd_mixer_selem_get_playback_volume(elem, selem_channel, &volume_cur);
        snd_mixer_selem_get_playback_volume_range(elem, &volume_min, &volume_max);
        volume = int (volume_cur * 100 / (volume_max - volume_min));
        my_bar_config.output.integer = volume;
        my_bar_config.output.output = to_string (volume) + "%";
    } else {
        my_bar_config.output.integer = -1;
        my_bar_config.output.output = "0%";
    }
    return_value = 0;
close:
    snd_mixer_close(handle);
    return return_value;
}
