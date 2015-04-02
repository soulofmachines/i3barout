#ifndef CLASSASOUND_HPP
#define CLASSASOUND_HPP

#include <alsa/asoundlib.h>
#include "classbase.hpp"

class classAsound : public classBase {
private:
    int                                 volume,
    volumeUnmute;
    long                                volumeCurrent,
    volumeMax,
    volumeMin;
    snd_mixer_t*                        handle;
    snd_mixer_selem_id_t*               selem_id;
    snd_mixer_elem_t*                   elem;
    const snd_mixer_selem_channel_id_t  selem_channel_id = SND_MIXER_SCHN_FRONT_LEFT;
    std::string                         device,
    mixer;
    unsigned int padding;
    bool mic;
public:
    classAsound();
    void readCustomConfig(yajl_val &config);
    void update();
};

#endif //CLASSASOUND_HPP
