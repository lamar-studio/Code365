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

#ifndef devicewidget_h
#define devicewidget_h

#include "pavucontrol.h"

class MainWindow;

class DeviceWidget {
public:
    DeviceWidget();
    virtual void updateChannelVolume(int channel, pa_volume_t v, bool isAll);

    std::string name;
    std::string description;
    std::string prio_type;
    std::string activePort;
    std::vector< std::pair<std::string,std::string> > ports;
    uint32_t index, card_index;
    uint32_t usb_cnt, hdmi_cnt, analog_cnt;

    pa_cvolume volume;

    virtual void executeVolumeUpdate();


    void prepareMenu();

protected:

private:

};

#endif
