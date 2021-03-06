/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2009 Colin Guthrie

  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pavucontrol. If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef audiomanager_h
#define audiomanager_h

#include <signal.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <libintl.h>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <pulse/pulseaudio.h>

#include "audiocore.h"

#define DEBUG     (1)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)

pa_context* get_context(void);

class AudioCore;
class listInfo;

class AudioManager
{
public:
    AudioManager();

    static AudioManager* getInstance();
    static void* main_loop(void *arg);
    int startPaService();
    int stopPaService();

    void printDeviceList();
    int setSoundDevicePath(uint32_t index, std::string name, std::string direction);
    int setSoundVolume(uint32_t volume, std::string direction);

protected:

private:
    static AudioManager *mInstance;
    static AudioCore *mAC;
    std::vector<listInfo> devList;


};


#endif
