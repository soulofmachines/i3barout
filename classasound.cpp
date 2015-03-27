#include "classasound.hpp"
#include "json.hpp"
#include "string.hpp"

classAsound::classAsound() {
}

void classAsound::readCustomConfig(yajl_val &config) {
    device = jsonGetString(config, "device", "default");
    mixer = jsonGetString(config, "mixer", "Master");
    padding = jsonGetInt(config, "padding", 3);
}

void classAsound::update() {
    integer = 0;
    output.clear();
    error.clear();
    if (snd_mixer_open(&handle, 0) < 0) {
        error = "Asound: open";
        goto end;
    }
    if (snd_mixer_attach(handle, device.c_str()) < 0) {
        error = "Asound: device";
        goto end;
    }
    if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
        error = "Asound: register";
        goto end;
    }
    if (snd_mixer_load(handle) < 0) {
        error = "Asound: load";
        goto end;
    }
    snd_mixer_selem_id_alloca(&selem_id);
    snd_mixer_selem_id_set_index(selem_id, 0);
    snd_mixer_selem_id_set_name(selem_id, mixer.c_str());
    elem = snd_mixer_find_selem(handle, selem_id);
    if (!elem) {
        error = "Asound: mixer";
        goto end;
    }
    snd_mixer_selem_get_playback_switch(elem, selem_channel_id, &volumeUnmute);
    if (volumeUnmute) {
        snd_mixer_selem_get_playback_volume(elem, selem_channel_id, &volumeCurrent);
        snd_mixer_selem_get_playback_volume_range(elem, &volumeMin, &volumeMax);
        integer = int (volumeCurrent * 100 / (volumeMax - volumeMin));
        output = std::to_string(integer);
    } else {
        integer = -1;
        output = "0";
    }
    stringPadZero(output, padding);
    output += "%";
end:
    snd_mixer_close(handle);
}
