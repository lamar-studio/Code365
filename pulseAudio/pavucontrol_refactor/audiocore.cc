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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>

#include "audiocore.h"

#include "card.h"
#include "sink.h"
#include "source.h"
#include "sinkinput.h"
#include "sourceoutput.h"

/* Used for profile sorting */
struct profile_prio_compare {
    bool operator() (const pa_card_profile_info& lhs, const pa_card_profile_info& rhs) const {

        if (lhs.priority == rhs.priority)
            return strcmp(lhs.name, rhs.name) > 0;

        return lhs.priority > rhs.priority;
    }
};

struct sink_port_prio_compare {
    bool operator() (const pa_sink_port_info& lhs, const pa_sink_port_info& rhs) const {

        if (lhs.priority == rhs.priority)
            return strcmp(lhs.name, rhs.name) > 0;

        return lhs.priority > rhs.priority;
    }
};

struct source_port_prio_compare {
    bool operator() (const pa_source_port_info& lhs, const pa_source_port_info& rhs) const {

        if (lhs.priority == rhs.priority)
            return strcmp(lhs.name, rhs.name) > 0;

        return lhs.priority > rhs.priority;
    }
};

AudioCore::AudioCore() :
    showSinkInputType(SINK_INPUT_CLIENT),
    showSinkType(SINK_ALL),
    showSourceOutputType(SOURCE_OUTPUT_CLIENT),
    showSourceType(SOURCE_NO_MONITOR) {

}

AudioCore::~AudioCore() {

}

static void updatePorts(Device *w, std::map<std::string, PortInfo> &ports) {
    std::map<std::string, PortInfo>::iterator it;
    PortInfo p;
    mlog("[linzr]enter:%s", __FUNCTION__);
    for (uint32_t i = 0; i < w->ports.size(); i++) {
        std::string desc;
        it = ports.find(w->ports[i].first);

        if (it == ports.end())
            continue;

        p = it->second;
        desc = p.description;

        if (p.available == PA_PORT_AVAILABLE_YES) {
            desc +=  " (plugged in)";
        }
        else if (p.available == PA_PORT_AVAILABLE_NO) {
            if (p.name == "analog-output-speaker" ||
                p.name == "analog-input-microphone-internal")
                desc += " (unavailable)";
            else
                desc += " (unplugged)";
        }

        w->ports[i].second = desc;
    }

    it = ports.find(w->activePort);

}

uint32_t AudioCore::sinkNameToIndex(std::string name) {

    Sink *sink;

    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it) {
        Sink *sink = it->second;

        if (sink->name == name) {
            return sink->index;
        }
    }
}

uint32_t AudioCore::sourceNameToIndex(std::string name) {

    Source *source;

    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it) {
        Source *source = it->second;

        if (source->name == name) {
            return source->index;
        }
    }
}

void AudioCore::updateCard(const pa_card_info &info) {
    Card *w;
    bool is_new = false;
    bool is_first_hdmi = true;
    bool is_first_analog = true;
    bool is_avalible = false;
    std::set<pa_card_profile_info, profile_prio_compare> profile_priorities;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if (cards.count(info.index))
        w = cards[info.index];
    else {
        cards[info.index] = w = new Card();
        w->index = info.index;
        is_new = true;
    }

    w->hasSinks = w->hasSources = false;

    profile_priorities.clear();
    for (uint32_t i=0; i<info.n_profiles; ++i) {
        w->hasSinks = w->hasSinks || (info.profiles[i].n_sinks > 0);
        w->hasSources = w->hasSources || (info.profiles[i].n_sources > 0);
        profile_priorities.insert(info.profiles[i]);
    }

    //ports info
    w->ports.clear();
    for (uint32_t i = 0; i < info.n_ports; ++i) {
        PortInfo p;

        p.name = info.ports[i]->name;
        p.description = info.ports[i]->description;
        p.priority = info.ports[i]->priority;
        p.available = info.ports[i]->available;
        p.direction = info.ports[i]->direction;
        for (uint32_t j = 0; j < info.ports[i]->n_profiles; j++)
            p.profiles.push_back(info.ports[i]->profiles[j]->name);

        w->ports[p.name] = p;
    }

    //profiles info
    w->profiles.clear();
    for (std::set<pa_card_profile_info>::iterator profileIt = profile_priorities.begin(); profileIt != profile_priorities.end(); ++profileIt) {
        bool hasNo = false, hasOther = false;
        std::map<std::string, PortInfo>::iterator portIt;
        std::string desc = profileIt->description;

        for (portIt = w->ports.begin(); portIt != w->ports.end(); portIt++) {
            PortInfo port = portIt->second;

            //if (std::find(port.profiles.begin(), port.profiles.end(), profileIt->name) == port.profiles.end())
             //   continue;
            if (port.available == PA_PORT_AVAILABLE_YES)
                is_avalible = true;

            if (port.available == PA_PORT_AVAILABLE_NO)
                hasNo = true;
            else {
                hasOther = true;
                break;
            }

        }
        if (hasNo && !hasOther)
            desc += "(unplugged)";

        if (strstr(info.name, "pci") != NULL) {
            if (strstr(profileIt->name, "hdmi") != NULL
                && is_first_hdmi == true ) {
                hdmiProfile = profileIt->name;
                is_first_hdmi = false;
            }

            if (strstr(profileIt->name, "analog") != NULL
                && is_first_analog == true) {
                analogProfile = profileIt->name;
                is_first_analog = false;
            }
        }

        w->profiles.push_back(std::pair<std::string,std::string>(profileIt->name, desc));
    }

    w->prepareMenu();

    mlog("[linzr]exit:%s", __FUNCTION__);
}

bool AudioCore::updateSink(const pa_sink_info &info) {
    Sink *w;
    const char *t;
    bool is_new = false;
    std::map<uint32_t, Card*>::iterator cw;
    std::set<pa_sink_port_info,sink_port_prio_compare> port_priorities;
    mlog("[linzr]enter:%s name:%s desc:%s", __FUNCTION__, info.name, info.description);
    if (sinks.count(info.index))
        w = sinks[info.index];
    else {
        is_new = true;

        sinks[info.index] = w = new Sink();
        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.flags & PA_SINK_HARDWARE ? SINK_HARDWARE : SINK_VIRTUAL;

    port_priorities.clear();
    for (uint32_t i=0; i<info.n_ports; ++i) {
        port_priorities.insert(*info.ports[i]);
    }

    w->ports.clear();
    for (std::set<pa_sink_port_info>::iterator i = port_priorities.begin(); i != port_priorities.end(); ++i)
        w->ports.push_back(std::pair<std::string,std::string>(i->name, i->description));

    w->activePort = info.active_port ? info.active_port->name : "";

    w->autoDefault(this);

    cw = cards.find(info.card);
    if (cw != cards.end())
        updatePorts(w, cw->second->ports);

    w->prepareMenu();
    mlog("[linzr]exit:%s prio_type:%s", __FUNCTION__, w->prio_type.c_str());
    return is_new;
}

void AudioCore::updateSource(const pa_source_info &info) {
    Source *w;
    bool is_new = false;
    std::map<uint32_t, Card*>::iterator cw;
    std::set<pa_source_port_info,source_port_prio_compare> port_priorities;
    mlog("[linzr]enter:%s name:%s desc:%s", __FUNCTION__, info.name, info.description);

    if (sources.count(info.index))
        w = sources[info.index];
    else {
        sources[info.index] = w = new Source();

        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
        is_new = true;
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.monitor_of_sink != PA_INVALID_INDEX ? SOURCE_MONITOR : (info.flags & PA_SOURCE_HARDWARE ? SOURCE_HARDWARE : SOURCE_VIRTUAL);

    port_priorities.clear();
    for (uint32_t i=0; i<info.n_ports; ++i) {
        port_priorities.insert(*info.ports[i]);
    }

    w->ports.clear();
    for (std::set<pa_source_port_info>::iterator i = port_priorities.begin(); i != port_priorities.end(); ++i)
        w->ports.push_back(std::pair<std::string,std::string>(i->name, i->description));

    w->activePort = info.active_port ? info.active_port->name : "";

    w->autoDefault(this);
    cw = cards.find(info.card);

    if (cw != cards.end())
        updatePorts(w, cw->second->ports);

    w->prepareMenu();
    mlog("[linzr]exit:%s prio_type:%s", __FUNCTION__, w->prio_type.c_str());
    return;
}

void AudioCore::updateSinkInput(const pa_sink_input_info &info) {
    const char *t;
    SinkInput *w;
    bool is_new = false;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if ((t = pa_proplist_gets(info.proplist, "module-stream-restore.id"))) {
        if (strcmp(t, "sink-input-by-media-role:event") == 0) {
            log("Ignoring sink-input due to it being designated as an event and thus handled by the Event widget");
            return;
        }
    }

    if (sinkInputs.count(info.index)) {
        w = sinkInputs[info.index];
    } else {
        sinkInputs[info.index] = w = new SinkInput();
        w->index = info.index;
        w->clientIndex = info.client;
        w->name = info.name;
        is_new = true;
    }

    w->type = info.client != PA_INVALID_INDEX ? SINK_INPUT_CLIENT : SINK_INPUT_VIRTUAL;

    return;
}

void AudioCore::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutput *w;
    const char *app;
    bool is_new = false;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if ((app = pa_proplist_gets(info.proplist, PA_PROP_APPLICATION_ID)))
        if (strcmp(app, "org.PulseAudio.pavucontrol") == 0
            || strcmp(app, "org.gnome.VolumeControl") == 0
            || strcmp(app, "org.kde.kmixd") == 0)
            return;

    if (sourceOutputs.count(info.index))
        w = sourceOutputs[info.index];
    else {
        sourceOutputs[info.index] = w = new SourceOutput();

        w->index = info.index;
        w->clientIndex = info.client;
        w->name = info.name;
        is_new = true;
    }

    w->type = info.client != PA_INVALID_INDEX ? SOURCE_OUTPUT_CLIENT : SOURCE_OUTPUT_VIRTUAL;

    return;
}

void AudioCore::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    //move sinkInput to default
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        it->second->moveSinkInput(defaultSinkName.c_str());

    //move sourceOutput to default
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        it->second->moveSourceOutput(defaultSourceName.c_str());

    mlog("[linzr]enter:%s defSource:%s defSink:%s", __FUNCTION__, defaultSourceName.c_str(), defaultSinkName.c_str());
}

void AudioCore::removeCard(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!cards.count(index))
        return;

    delete cards[index];
    cards.erase(index);
}

void AudioCore::removeSink(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sinks.count(index))
        return;

    delete sinks[index];
    sinks.erase(index);
}

void AudioCore::removeSource(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sources.count(index))
        return;

    delete sources[index];
    sources.erase(index);
}

void AudioCore::removeSinkInput(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sinkInputs.count(index))
        return;

    delete sinkInputs[index];
    sinkInputs.erase(index);
}

void AudioCore::removeSourceOutput(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sourceOutputs.count(index))
        return;

    delete sourceOutputs[index];
    sourceOutputs.erase(index);
}

void AudioCore::removeAll() {
    mlog("[linzr]enter:%s", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        removeSinkInput(it->first);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        removeSourceOutput(it->first);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        removeSink(it->first);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        removeSource(it->first);
    for (std::map<uint32_t, Card*>::iterator it = cards.begin(); it != cards.end(); ++it)
       removeCard(it->first);
}

void AudioCore::printAll() {
    mlog("[linzr]enter:%s", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        mlog("sinkInput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        mlog("sourceOutput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        mlog("sink:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        mlog("source:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Card*>::iterator it = cards.begin(); it != cards.end(); ++it)
        mlog("card:%s (%d)", it->second->name.c_str(), it->second->index);

    mlog("[linzr]exit:%s hdmiprofile:%s analogProfile:%s", __FUNCTION__, hdmiProfile.c_str(), analogProfile.c_str());
}

//new interface
void AudioCore::updateDeviceList() {

    devList.clear();
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it) {
        Sink *w = it->second;
        listInfo l;

        if (w->name == defaultSinkName)
            l.def = "true";
        else
            l.def= "false";

        //virtual device for Built-in
        if (strstr(w->description.c_str(), "Built-in")) {
            l.index = w->index;
            l.direction = "OUT";
            l.name = analogProfile;
            l.description = "Built-in (Analog)";
            devList.push_back(l);

            if (!hdmiProfile.empty()) {
                l.name = hdmiProfile;
                l.description = "Built-in (HDMI)";
                devList.push_back(l);
            }

            continue;
        }

        l.index = w->index;
        l.name = w->name;
        l.description = w->description;
        l.direction = "OUT";

        devList.push_back(l);
    }

    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it) {
        Source *s = it->second;
        listInfo l;

        if (s->name == defaultSourceName)
            l.def = "true";
        else
            l.def = "false";

        l.index = s->index;
        l.name = s->name;
        l.description = s->description;
        l.direction = "IN";

        devList.push_back(l);
    }

}

std::vector<listInfo> AudioCore::getDeviceList() {

    updateDeviceList();
    return devList;
}

void AudioCore::changeProfile(uint32_t index, std::string name) { //profile.second

    if (sinks.count(index)) {
        Sink *sink = sinks[index];

        pa_operation* o;

        if (!(o = pa_context_set_card_profile_by_index(get_context(), sink->card_index, name.c_str(), NULL, NULL))) {
            log("pa_context_set_card_profile_by_index() failed");
            return;
        }
        pa_operation_unref(o);
    }
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


int AudioCore::setDefaultSink(uint32_t index) {

    if (sinks.count(index)) {
        Sink *sink = sinks[index];
        sink->updateDefault(sink->name.c_str());
    }

    return 0;
}

int AudioCore::setDefaultSource(uint32_t index) {

    if (sources.count(index)) {
        Source *source = sources[index];
        source->updateDefault(source->name.c_str());
    }

    return 0;
}


