
#ifndef sinkinput_h
#define sinkinput_h

#include "audiocore.h"

class SinkInput {
public:
    SinkInput();

    std::string name;
    uint32_t index;
    virtual void moveSinkInput(const char *defName);

private:
    uint32_t mSinkIndex;

};

#endif
