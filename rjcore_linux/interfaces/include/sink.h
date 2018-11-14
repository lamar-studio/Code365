
#ifndef sink_h
#define sink_h

#include "device.h"

class Sink : public Device {
public:
    Sink();
    virtual ~Sink();

    static int setVolume(std::string name, int percentage);
    static void updateDefault(const char *name);
    static void autoDefault(AudioCore *ac);

};

#endif
