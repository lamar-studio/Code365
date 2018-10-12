
#ifndef sourceoutput_h
#define sourceoutput_h

#include "audiocore.h"

class SourceOutput {
public:
    SourceOutput();

    std::string name;
    uint32_t index;
    virtual void moveSourceOutput(const char *defName);
};

#endif
