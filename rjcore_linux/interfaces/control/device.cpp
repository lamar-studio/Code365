
#include "audiocore.h"
#include "device.h"

pa_cvolume Device::volume;

Device::Device()
    : index(0),
      card_index(0) {

}

Device::~Device() {

}


