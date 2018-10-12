
#include "audiocore.h"
#include "device.h"

/*** Device ***/
Device::Device() {

}

void Device::updateChannelVolume(int channel, pa_volume_t v, bool isAll) {
    pa_cvolume n;
    //g_assert(channel < volume.channels);

    n = volume;
    if (isAll)
        pa_cvolume_set(&n, n.channels, v);
    else
        n.values[channel] = v;
}


