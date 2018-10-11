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

#include <set>

#include "audiocore.h"
#include "sink.h"
#include "source.h"
#include "sinkinput.h"
#include "sourceoutput.h"

AudioCore* AudioCore::mInstance = NULL;

AudioCore::AudioCore() {

}

AudioCore::~AudioCore() {

}

AudioCore* AudioCore::getInstance() {

    if (mInstance == NULL) {
        mInstance = new AudioCore();
    }

    return mInstance;
}

void AudioCore::updateSink(const pa_sink_info &info) {
    Sink *w;
    mlog("[%s]name:%s index:%d", __FUNCTION__, info.name, info.index);

    if (sinks.count(info.index))
        w = sinks[info.index];
    else {
        sinks[info.index] = w = new Sink();
        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->autoDefault(this);

    return;
}

void AudioCore::updateSource(const pa_source_info &info) {
    Source *w;
    mlog("[%s]name:%s desc:%s", __FUNCTION__, info.name, info.description);

    if (sources.count(info.index))
        w = sources[info.index];
    else {
        sources[info.index] = w = new Source();

        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->autoDefault(this);

    return;
}

void AudioCore::updateSinkInput(const pa_sink_input_info &info) {
    SinkInput *w;
    mlog("[%s]name:%s", __FUNCTION__, info.name);

    if (sinkInputs.count(info.index)) {
        w = sinkInputs[info.index];
    } else {
        sinkInputs[info.index] = w = new SinkInput();
        w->index = info.index;
        w->name = info.name;
    }

    return;
}

void AudioCore::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutput *w;
    mlog("[%s]name:%s", __FUNCTION__, info.name);

    if (sourceOutputs.count(info.index))
        w = sourceOutputs[info.index];
    else {
        sourceOutputs[info.index] = w = new SourceOutput();

        w->index = info.index;
        w->name = info.name;
    }

    return;
}

void AudioCore::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    mlog("[%s]defSource:%s defSink:%s", __FUNCTION__, defaultSourceName.c_str(), defaultSinkName.c_str());
}

void AudioCore::removeSink(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sinks.count(index))
        return;

    delete sinks[index];
    sinks.erase(index);
}

void AudioCore::removeSource(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sources.count(index))
        return;

    delete sources[index];
    sources.erase(index);
}

void AudioCore::removeSinkInput(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sinkInputs.count(index))
        return;

    delete sinkInputs[index];
    sinkInputs.erase(index);
}

void AudioCore::removeSourceOutput(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sourceOutputs.count(index))
        return;

    delete sourceOutputs[index];
    sourceOutputs.erase(index);
}

void AudioCore::removeAll() {
    mlog("[%s]", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        removeSinkInput(it->first);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        removeSourceOutput(it->first);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        removeSink(it->first);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        removeSource(it->first);
}

void AudioCore::printAll() {
    mlog("[%s]", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        mlog("sinkInput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        mlog("sourceOutput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        mlog("sink:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        mlog("source:%s (%d)", it->second->name.c_str(), it->second->index);
}

int AudioCore::setSinkVolume(pa_volume_t vol) {

    if (sinks.count(defaultSinkIdx)) {
        Sink *sink = sinks[defaultSinkIdx];
        sink->updateVolume(vol);
    }

    return 0;
}

int AudioCore::setSourceVolume(pa_volume_t vol) {

    if (sources.count(defaultSourceIdx)) {
        Source *source = sources[defaultSourceIdx];
        source->updateVolume(vol);
    }

    return 0;
}


