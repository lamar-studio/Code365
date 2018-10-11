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

#include "audiomanager.h"
#include <pulse/ext-stream-restore.h>


enum SinkInputType {
    SINK_INPUT_ALL,
    SINK_INPUT_CLIENT,
    SINK_INPUT_VIRTUAL
};

enum SinkType {
    SINK_ALL,
    SINK_HARDWARE,
    SINK_VIRTUAL,
};

enum SourceOutputType {
    SOURCE_OUTPUT_ALL,
    SOURCE_OUTPUT_CLIENT,
    SOURCE_OUTPUT_VIRTUAL
};

enum SourceType {
    SOURCE_ALL,
    SOURCE_NO_MONITOR,
    SOURCE_HARDWARE,
    SOURCE_VIRTUAL,
    SOURCE_MONITOR,
};

enum VolumeDirection {
    SINK,
    SOURCE,
};


class Card;
class Sink;
class Source;
class SinkInput;
class SourceOutput;

class listInfo {
public:
      uint32_t index;
      std::string name;
      std::string description;
      std::string def;
      std::string direction;
      std::string status;
};


class AudioCore {
public:
    AudioCore();
    virtual ~AudioCore();

    void updateCard(const pa_card_info &info);
    bool updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);
    void updateSourceOutput(const pa_source_output_info &info);
    void updateServer(const pa_server_info &info);

    void removeCard(uint32_t index);
    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    void removeSourceOutput(uint32_t index);
    void removeClient(uint32_t index);

    void removeAll();
    void printAll();

    void updateDeviceList();
    std::vector<listInfo> getDeviceList();
    void changeProfile(uint32_t index, std::string name);
    int setSinkVolume(pa_volume_t vol);
    int setSourceVolume(pa_volume_t vol);
    int setDefaultSink(uint32_t index);
    int setDefaultSource(uint32_t index);

    std::map<uint32_t, Card*, std::greater<uint32_t>> cards;
    std::map<uint32_t, Sink*, std::greater<uint32_t>> sinks;
    std::map<uint32_t, Source*, std::greater<uint32_t>> sources;
    std::map<uint32_t, SinkInput*, std::greater<uint32_t>> sinkInputs;
    std::map<uint32_t, SourceOutput*, std::greater<uint32_t>> sourceOutputs;
    std::vector<listInfo> devList;

    SinkInputType showSinkInputType;
    SinkType showSinkType;
    SourceOutputType showSourceOutputType;
    SourceType showSourceType;

    std::string defaultSinkName, defaultSourceName;
    uint32_t defaultSinkIdx, defaultSourceIdx;
protected:

private:
    bool mManual;
    std::string hdmiProfile;
    std::string analogProfile;
    uint32_t sinkNameToIndex(std::string name);
    uint32_t sourceNameToIndex(std::string name);

};


#endif
