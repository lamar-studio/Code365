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

#include "cardwidget.h"

/*** CardWidget ***/
CardWidget::CardWidget() {

}

void CardWidget::prepareMenu() {

    /* Fill the ComboBox's Tree Model */
    for (uint32_t i = 0; i < profiles.size(); ++i) {
        //log("profile.fist:%s profile.second:[%s]", profiles[i].first.c_str(), profiles[i].second.c_str());
        log("profiles:[%s]", profiles[i].second.c_str());
    }

}

void CardWidget::onProfileChange(uint32_t index, const char *profile) { //profile.second

    if (updating)
        return;

    pa_operation* o;

    if (!(o = pa_context_set_card_profile_by_index(get_context(), index, profile, NULL, NULL))) {
        log("pa_context_set_card_profile_by_index() failed");
        return;
    }
    pa_operation_unref(o);

}
