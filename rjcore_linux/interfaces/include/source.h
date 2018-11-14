
#ifndef source_h
#define source_h

#include "device.h"

class Source : public Device {
public:
    Source();
    virtual ~Source();

    static int updateVolume(std::string name, int percentage);
    static void updateDefault(const char *name);
    static void autoDefault(AudioCore *ac);
};

#endif
