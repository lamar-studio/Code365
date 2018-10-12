
#ifndef source_h
#define source_h

#include "device.h"

class Source : public Device {
public:
    Source();

    virtual void updateVolume(pa_volume_t v);
    virtual void updateDefault(const char *name);
    virtual void autoDefault(AudioCore *ac);
};

#endif
