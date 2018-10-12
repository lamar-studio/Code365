
#ifndef audiomanager_h
#define audiomanager_h

#include "audiocore.h"

class AudioCore;

class AudioManager
{
public:
    AudioManager();

    int startPaService();
    int stopPaService();
    void print();
};


#endif
