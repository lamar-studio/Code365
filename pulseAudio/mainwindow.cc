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

#include "mainwindow.h"

#include "cardwidget.h"
#include "sinkwidget.h"
#include "sourcewidget.h"
#include "sinkinputwidget.h"
#include "sourceoutputwidget.h"

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

MainWindow::MainWindow() :
    showSinkInputType(SINK_INPUT_CLIENT),
    showSinkType(SINK_ALL),
    showSourceOutputType(SOURCE_OUTPUT_CLIENT),
    showSourceType(SOURCE_NO_MONITOR) {

}

MainWindow::~MainWindow() {

}

static void updatePorts(DeviceWidget *w, std::map<std::string, PortInfo> &ports) {
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

        if (p.available == PA_PORT_AVAILABLE_YES)
            desc +=  " (plugged in)";
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

void MainWindow::updateCard(const pa_card_info &info) {
    CardWidget *w;
    bool is_new = false;
    std::set<pa_card_profile_info, profile_prio_compare> profile_priorities;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if (cardWidgets.count(info.index))
        w = cardWidgets[info.index];
    else {
        cardWidgets[info.index] = w = new CardWidget();
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

    //保存端口信息
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

    //保存列表,并更新设备的连接状态
    w->profiles.clear();
    for (std::set<pa_card_profile_info>::iterator profileIt = profile_priorities.begin(); profileIt != profile_priorities.end(); ++profileIt) {
        bool hasNo = false, hasOther = false;
        std::map<std::string, PortInfo>::iterator portIt;
        std::string desc = profileIt->description;

        for (portIt = w->ports.begin(); portIt != w->ports.end(); portIt++) {
            PortInfo port = portIt->second;

            //if (std::find(port.profiles.begin(), port.profiles.end(), profileIt->name) == port.profiles.end())
            //    continue;

            if (port.available == PA_PORT_AVAILABLE_NO)
                hasNo = true;
            else {
                hasOther = true;
                break;
            }
        }
        if (hasNo && !hasOther)
            desc += "(unplugged)";

        w->profiles.push_back(std::pair<std::string,std::string>(profileIt->name, desc));
    }

    w->activeProfile = info.active_profile ? info.active_profile->name : "";

    /* Because the port info for sinks and sources is discontinued we need
     * to update the port info for them here. */
    if (w->hasSinks) {
        std::map<uint32_t, SinkWidget*>::iterator it;

        for (it = sinkWidgets.begin() ; it != sinkWidgets.end(); it++) {
            SinkWidget *sw = it->second;

            if (sw->card_index == w->index) {
                updatePorts(sw, w->ports);
            }
        }
    }

    if (w->hasSources) {
        std::map<uint32_t, SourceWidget*>::iterator it;

        for (it = sourceWidgets.begin() ; it != sourceWidgets.end(); it++) {
            SourceWidget *sw = it->second;

            if (sw->card_index == w->index) {
                updatePorts(sw, w->ports);
            }
        }
    }

    w->prepareMenu();

    // if (is_new)
        // updateDeviceVisibility(); //可以换做其他的逻辑判断
    mlog("[linzr]exit:%s", __FUNCTION__);
}

bool MainWindow::updateSink(const pa_sink_info &info) {
    SinkWidget *w;
    const char *t;
    bool is_new = false;
    std::map<uint32_t, CardWidget*>::iterator cw;
    std::set<pa_sink_port_info,sink_port_prio_compare> port_priorities;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if (sinkWidgets.count(info.index))
        w = sinkWidgets[info.index];
    else {
        is_new = true;

        sinkWidgets[info.index] = w = new SinkWidget();
        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.flags & PA_SINK_HARDWARE ? SINK_HARDWARE : SINK_VIRTUAL;

    if (!mManual)
        w->autoDefault();

    port_priorities.clear();
    for (uint32_t i=0; i<info.n_ports; ++i) {
        port_priorities.insert(*info.ports[i]);
    }

    w->ports.clear();
    for (std::set<pa_sink_port_info>::iterator i = port_priorities.begin(); i != port_priorities.end(); ++i)
        w->ports.push_back(std::pair<std::string,std::string>(i->name, i->description));

    w->activePort = info.active_port ? info.active_port->name : "";

    cw = cardWidgets.find(info.card);
    if (cw != cardWidgets.end())
        updatePorts(w, cw->second->ports);

    w->prepareMenu();
    mlog("[linzr]exit:%s prio_type:%s", __FUNCTION__, w->prio_type.c_str());
    return is_new;
}

void MainWindow::updateSource(const pa_source_info &info) {
    SourceWidget *w;
    bool is_new = false;
    std::map<uint32_t, CardWidget*>::iterator cw;
    std::set<pa_source_port_info,source_port_prio_compare> port_priorities;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);

    if (sourceWidgets.count(info.index))
        w = sourceWidgets[info.index];
    else {
        sourceWidgets[info.index] = w = new SourceWidget();

        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
        is_new = true;
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->type = info.monitor_of_sink != PA_INVALID_INDEX ? SOURCE_MONITOR : (info.flags & PA_SOURCE_HARDWARE ? SOURCE_HARDWARE : SOURCE_VIRTUAL);

    if (!mManual)
        w->autoDefault();

    port_priorities.clear();
    for (uint32_t i=0; i<info.n_ports; ++i) {
        port_priorities.insert(*info.ports[i]);
    }

    w->ports.clear();
    for (std::set<pa_source_port_info>::iterator i = port_priorities.begin(); i != port_priorities.end(); ++i)
        w->ports.push_back(std::pair<std::string,std::string>(i->name, i->description));

    w->activePort = info.active_port ? info.active_port->name : "";

    cw = cardWidgets.find(info.card);

    if (cw != cardWidgets.end())
        updatePorts(w, cw->second->ports);

    w->prepareMenu();
    mlog("[linzr]exit:%s prio_type:%s", __FUNCTION__, w->prio_type.c_str());
    return;
}

void MainWindow::updateSinkInput(const pa_sink_input_info &info) {
    const char *t;
    SinkInputWidget *w;
    bool is_new = false;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if ((t = pa_proplist_gets(info.proplist, "module-stream-restore.id"))) {
        if (strcmp(t, "sink-input-by-media-role:event") == 0) {
            log("Ignoring sink-input due to it being designated as an event and thus handled by the Event widget");
            return;
        }
    }

    if (sinkInputWidgets.count(info.index)) {
        w = sinkInputWidgets[info.index];
    } else {
        sinkInputWidgets[info.index] = w = new SinkInputWidget();
        w->index = info.index;
        w->clientIndex = info.client;
        w->name = info.name;
        is_new = true;
    }

    w->type = info.client != PA_INVALID_INDEX ? SINK_INPUT_CLIENT : SINK_INPUT_VIRTUAL;

    return;
}

void MainWindow::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutputWidget *w;
    const char *app;
    bool is_new = false;
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, info.name);
    if ((app = pa_proplist_gets(info.proplist, PA_PROP_APPLICATION_ID)))
        if (strcmp(app, "org.PulseAudio.pavucontrol") == 0
            || strcmp(app, "org.gnome.VolumeControl") == 0
            || strcmp(app, "org.kde.kmixd") == 0)
            return;

    if (sourceOutputWidgets.count(info.index))
        w = sourceOutputWidgets[info.index];
    else {
        sourceOutputWidgets[info.index] = w = new SourceOutputWidget();

        w->index = info.index;
        w->clientIndex = info.client;
        w->name = info.name;
        is_new = true;
    }

    w->type = info.client != PA_INVALID_INDEX ? SOURCE_OUTPUT_CLIENT : SOURCE_OUTPUT_VIRTUAL;
    w->moveSourceOutput(defaultSourceIdx);

    return;
}

void MainWindow::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    for (std::map<uint32_t, SinkInputWidget*>::iterator it = sinkInputWidgets.begin(); it != sinkInputWidgets.end(); ++it)
        it->second->moveSinkInput(defaultSinkName.c_str());
    mlog("[linzr]enter:%s defSink:%s defSource:%s", __FUNCTION__, defaultSourceName.c_str(), defaultSinkName.c_str());
}

void MainWindow::removeCard(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!cardWidgets.count(index))
        return;

    delete cardWidgets[index];
    cardWidgets.erase(index);
}

void MainWindow::removeSink(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sinkWidgets.count(index))
        return;

    delete sinkWidgets[index];
    sinkWidgets.erase(index);
}

void MainWindow::removeSource(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sourceWidgets.count(index))
        return;

    delete sourceWidgets[index];
    sourceWidgets.erase(index);
}

void MainWindow::removeSinkInput(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sinkInputWidgets.count(index))
        return;

    delete sinkInputWidgets[index];
    sinkInputWidgets.erase(index);
}

void MainWindow::removeSourceOutput(uint32_t index) {
    mlog("[linzr]enter:%s index:%d", __FUNCTION__, index);
    if (!sourceOutputWidgets.count(index))
        return;

    delete sourceOutputWidgets[index];
    sourceOutputWidgets.erase(index);
}

void MainWindow::removeAllWidgets() {
    mlog("[linzr]enter:%s", __FUNCTION__);
    for (std::map<uint32_t, SinkInputWidget*>::iterator it = sinkInputWidgets.begin(); it != sinkInputWidgets.end(); ++it)
        removeSinkInput(it->first);
    for (std::map<uint32_t, SourceOutputWidget*>::iterator it = sourceOutputWidgets.begin(); it != sourceOutputWidgets.end(); ++it)
        removeSourceOutput(it->first);
    for (std::map<uint32_t, SinkWidget*>::iterator it = sinkWidgets.begin(); it != sinkWidgets.end(); ++it)
        removeSink(it->first);
    for (std::map<uint32_t, SourceWidget*>::iterator it = sourceWidgets.begin(); it != sourceWidgets.end(); ++it)
        removeSource(it->first);
    for (std::map<uint32_t, CardWidget*>::iterator it = cardWidgets.begin(); it != cardWidgets.end(); ++it)
       removeCard(it->first);
}

void MainWindow::printAllWidgets() {
    mlog("[linzr]enter:%s", __FUNCTION__);
    for (std::map<uint32_t, SinkInputWidget*>::iterator it = sinkInputWidgets.begin(); it != sinkInputWidgets.end(); ++it)
        mlog("sinkInput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SourceOutputWidget*>::iterator it = sourceOutputWidgets.begin(); it != sourceOutputWidgets.end(); ++it)
        mlog("sourceOutput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SinkWidget*>::iterator it = sinkWidgets.begin(); it != sinkWidgets.end(); ++it)
        mlog("sink:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SourceWidget*>::iterator it = sourceWidgets.begin(); it != sourceWidgets.end(); ++it)
        mlog("source:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, CardWidget*>::iterator it = cardWidgets.begin(); it != cardWidgets.end(); ++it)
        mlog("card:%s (%d)", it->second->name.c_str(), it->second->index);
}

//new interface
int MainWindow::getDeviceList(char *list) {

    for (std::map<uint32_t, CardWidget*>::iterator it = cardWidgets.begin(); it != cardWidgets.end(); ++it) {
        CardWidget *c = it->second;
        log("CARD[%d]:", c->index);
        for (uint32_t i = 0; i < c->profiles.size(); ++i) {
            log("profile.name:%s profile.desc:%s", c->profiles[i].first.c_str(), c->profiles[i].second.c_str());
        }
    }
}

void MainWindow::setSoundPath(uint32_t index, const char *profile) { //profile.second

    mManual = true;
    pa_operation* o;

    if (!(o = pa_context_set_card_profile_by_index(get_context(), index, profile, NULL, NULL))) {
        log("pa_context_set_card_profile_by_index() failed");
        return;
    }
    pa_operation_unref(o);

}

int MainWindow::setSoundVolume(uint32_t volume, int direction) { //only change the default

    if (sinkWidgets.count(defaultSinkIdx)) {
        SinkWidget *sink = sinkWidgets[defaultSinkIdx];
        sink->setVolume(volume);
    }

    if (sourceWidgets.count(defaultSourceIdx)) {
        SourceWidget *source = sourceWidgets[defaultSourceIdx];
        source->setVolume(volume);
    }

    return 0;
}

void MainWindow::updateDefaultIdx() { //only change the default

    SinkWidget *sink;
    SourceWidget *source;

    for (std::map<uint32_t, SinkWidget*>::iterator it = sinkWidgets.begin(); it != sinkWidgets.end(); ++it) {
        SinkWidget *sink = it->second;

        if (sink->name == defaultSinkName) {
            defaultSinkIdx = sink->index;
            break;
        }
    }

    for (std::map<uint32_t, SourceWidget*>::iterator it = sourceWidgets.begin(); it != sourceWidgets.end(); ++it) {
        SourceWidget *source = it->second;

        if (source->name == defaultSourceName) {
            defaultSourceIdx = source->index;
            break;
        }
    }

}


