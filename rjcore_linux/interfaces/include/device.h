
#ifndef device_h
#define device_h

#include "audiocore.h"

class AudioCore;

class Device {
public:
    Device();
    virtual ~Device();

    std::string name;
    std::string description;
    uint32_t index, card_index;
    std::string prio_type, vendor_id, product_id;

protected:
    static pa_cvolume volume;
};

#endif
