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

#include "sinkinput.h"
#include "audiocore.h"
#include "sink.h"

SinkInput::SinkInput() {

}


SinkInput::~SinkInput(void) {

}

uint32_t SinkInput::sinkIndex() {
    return mSinkIndex;
}

void SinkInput::moveSinkInput(const char *defName) {

  pa_operation* o;
  if (!(o = pa_context_move_sink_input_by_name(get_context(), index, defName, NULL, NULL))) {
    log("pa_context_move_sink_input_by_index() failed");
    return;
  }

  pa_operation_unref(o);
}

