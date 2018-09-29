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

#include "sinkwidget.h"

#if HAVE_EXT_DEVICE_RESTORE_API
#  include <pulse/format.h>
#  include <pulse/ext-device-restore.h>
#endif

SinkWidget::SinkWidget() {

}


void SinkWidget::setVolume(pa_volume_t v) {
    //updateChannelVolume(0, v, true);
}

void SinkWidget::executeVolumeUpdate() {
    pa_operation* o;

    if (!(o = pa_context_set_sink_volume_by_index(get_context(), index, &volume, NULL, NULL))) {
        log("pa_context_set_sink_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void SinkWidget::autoDefault() {

    std::string usb_name;
    std::string hdmi_name;
    std::string ana_name;
    std::string def_name;

    uint32_t idx = 0;
#if 0
    for (std::map<uint32_t, SinkWidget*>::iterator i = sinkWidgets.begin(); i != sinkWidgets.end(); ++i) {
        SinkWidget *w = i->second;
        if (!w)
            continue;

        if (strstr(w->prio_type.c_str(), "USB") != NULL || strstr(w->prio_type.c_str(), "usb") != NULL) {
            def_name = w->name;
            break;
        } else if (strstr(w->prio_type.c_str(), "HDMI") != NULL || strstr(w->prio_type.c_str(), "hdmi") != NULL) {
            if (hdmi_name.empty())
                hdmi_name = w->name;
            continue;
        } else {
            if (ana_name.empty())
                ana_name = w->name;
            continue;
        }
    }

    if (def_name.empty()) {
        def_name = hdmi_name.empty() ? ana_name : hdmi_name;
    }

    updateDefault(def_name.c_str());
#endif

}


void SinkWidget::manualDefault() {

    const char *usb_name = NULL;
    const char *hdmi_name = NULL;
    const char *ana_name = NULL;
    const char *def_name = NULL;
    uint32_t idx = 0;

    //updateDefault(name);

}

void SinkWidget::updateDefault(const char *name) {
    pa_operation* o;

    if (!(o = pa_context_set_default_sink(get_context(), name, NULL, NULL))) {
        log("pa_context_set_default_sink() failed");
        return;
    }
    pa_operation_unref(o);
}


