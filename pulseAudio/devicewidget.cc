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

#include <pulse/ext-device-manager.h>

#include "mainwindow.h"
#include "devicewidget.h"

/*** DeviceWidget ***/
DeviceWidget::DeviceWidget() {

}

void DeviceWidget::updateChannelVolume(int channel, pa_volume_t v, bool isAll) {
    pa_cvolume n;
    //g_assert(channel < volume.channels);

    n = volume;
    if (isAll)
        pa_cvolume_set(&n, n.channels, v);
    else
        n.values[channel] = v;

    executeVolumeUpdate();
}

#if 0
void DeviceWidget::setDefault(bool isDefault) {

#if 0
    //根据优先级策略,选出idx
    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        SinkWidget *w = i->second;
        fprintf(stderr, "enter:%s --linzaorong sinkName:%s\n", __FUNCTION__, i->second->description.c_str());
        if (!w)
            continue;

        w->updating = true;
        w->setDefault(w->name == defaultSinkName);  //更新图标

        w->updating = false;
    }

    for (std::map<uint32_t, SourceWidget*>::iterator i = sourceWidgets.begin(); i != sourceWidgets.end(); ++i) {
        SourceWidget *w = i->second;
        fprintf(stderr, "enter:%s --linzaorong sourceName:%s\n", __FUNCTION__, i->second->description.c_str());

        if (!w)
            continue;
        w->updating = true;
        w->setDefault(w->name == defaultSourceName);
        w->updating = false;
    }

    updateDefault();
#endif
    return;
}
#endif

void DeviceWidget::executeVolumeUpdate() {

}


void DeviceWidget::prepareMenu() {

    log("activePort:%s", activePort.c_str());
    /* Fill the ComboBox's Tree Model */
    for (uint32_t i = 0; i < ports.size(); ++i) {

        //log("prots.first:%s prots.second:%s", ports[i].first.c_str(), ports[i].second.c_str());
        log("prots:[%s]", ports[i].second.c_str());
    }
}


