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

#ifndef audiocore_h
#define audiocore_h

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

#define DEBUG     (1)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)

pa_context* get_context(void);


class Sink;
class Source;
class SinkInput;
class SourceOutput;

class AudioCore {
public:
    AudioCore();
    virtual ~AudioCore();
    static AudioCore* getInstance();

    void updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);
    void updateSourceOutput(const pa_source_output_info &info);
    void updateServer(const pa_server_info &info);

    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    void removeSourceOutput(uint32_t index);

    void removeAll();
    void printAll();

    int setSinkVolume(pa_volume_t vol);
    int setSourceVolume(pa_volume_t vol);

    std::map<uint32_t, Sink*, std::greater<uint32_t>> sinks;
    std::map<uint32_t, Source*, std::greater<uint32_t>> sources;
    std::map<uint32_t, SinkInput*, std::greater<uint32_t>> sinkInputs;
    std::map<uint32_t, SourceOutput*, std::greater<uint32_t>> sourceOutputs;

protected:

private:
    std::string defaultSinkName, defaultSourceName;
    uint32_t defaultSinkIdx, defaultSourceIdx;
    static AudioCore* mInstance;

};


#endif
