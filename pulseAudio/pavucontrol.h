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

#ifndef pavucontrol_h
#define pavucontrol_h

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

#include <pulse/pulseaudio.h>

#ifndef GLADE_FILE
#define GLADE_FILE "pavucontrol.glade"
#endif

/* Can be removed when PulseAudio 0.9.23 or newer is required */
#ifndef PA_VOLUME_UI_MAX
# define PA_VOLUME_UI_MAX (pa_sw_volume_from_dB(+11.0))
#endif

#define HAVE_SOURCE_OUTPUT_VOLUMES PA_CHECK_VERSION(0,99,0)
#define HAVE_EXT_DEVICE_RESTORE_API PA_CHECK_VERSION(0,99,0)

#define DEBUG     (1)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)


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

pa_context* get_context(void);

#endif
