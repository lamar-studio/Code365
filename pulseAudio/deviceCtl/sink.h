
#ifndef sink_h
#define sink_h

#include "device.h"

class Sink : public Device {
public:
    Sink();
    virtual ~Sink();

    virtual void updateVolume(pa_volume_t v);
    virtual void updateDefault(const char *name);
    virtual void autoDefault(AudioCore *ac);
};

#endif
