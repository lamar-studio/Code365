/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2008 Sjoerd Simons <sjoerd@luon.net>

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

#include <unistd.h>
#include <thread>
#include <pulse/pulseaudio.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/ext-device-manager.h>

#include "audiomanager.h"
#include "card.h"
#include "sink.h"
#include "source.h"
#include "sinkinput.h"
#include "sourceoutput.h"
#include "audiocore.h"

static pa_context* context = NULL;
static pa_mainloop_api* api = NULL;
static int n_outstanding = 0;
static int default_tab = 0;
static bool retry = false;
static int reconnect_timeout = 1;
static bool reconnect_running = false;
static bool connected = false;

#define DEBUG     (1)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)

void card_cb(pa_context *, const pa_card_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s eol:%d", __FUNCTION__, eol);
    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        log("Card callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateCard(*i);
}

void sink_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s eol:%d", __FUNCTION__, eol);
    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        log("Sink callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateSink(*i);
}

void source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s eol:%d", __FUNCTION__, eol);
    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        log("Source callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    //if (strstr(i->name, "monitor") != NULL)
    //    return;

    if (i->monitor_of_sink != PA_INVALID_INDEX)
        return;

    w->updateSource(*i);
}

void sink_input_cb(pa_context *, const pa_sink_input_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s eol:%d", __FUNCTION__, eol);
    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        log("Sink input callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateSinkInput(*i);
}

void source_output_cb(pa_context *, const pa_source_output_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s eol:%d", __FUNCTION__, eol);
    if (eol < 0) {
        if (pa_context_errno(context) == PA_ERR_NOENTITY)
            return;

        log("Source output callback failure");
        return;
    }

    if (eol > 0) {
        w->printAll();
        return;
    }

    w->updateSourceOutput(*i);
}

void server_info_cb(pa_context *, const pa_server_info *i, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s name:%s", __FUNCTION__, i->server_name);
    if (!i) {
        log("Server info callback failure");
        return;
    }

    w->updateServer(*i);
}

void subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s case:%#x", __FUNCTION__, t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK);
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK: // 0x0000U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSink(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_sink_info_by_index(c, index, sink_cb, w))) {
                    log("pa_context_get_sink_info_by_index() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE: //0x0001U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSource(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_source_info_by_index(c, index, source_cb, w))) {
                    log("pa_context_get_source_info_by_index() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SINK_INPUT: //0x0002U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSinkInput(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_sink_input_info(c, index, sink_input_cb, w))) {
                    log("pa_context_get_sink_input_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT: //0x0003U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeSourceOutput(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_source_output_info(c, index, source_output_cb, w))) {
                    log("pa_context_get_sink_input_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SERVER: { //0x0007U
                pa_operation *o;
                if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                    log("pa_context_get_server_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_CARD: //0x0009U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                w->removeCard(index);
            else {
                pa_operation *o;
                if (!(o = pa_context_get_card_info_by_index(c, index, card_cb, w))) {
                    log("pa_context_get_card_info_by_index() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

    }
}

/* Forward Declaration */
bool connect_to_pulse(void *userdata);

void do_reconnect(void *userdata)
{
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (reconnect_running || connected) {
        return;
    }

    reconnect_running = true;

    while (!connected) {
        connect_to_pulse(w);
        usleep(500000);
    }

    reconnect_running = false;
}


void reconnect(void *userdata)
{

    if (!reconnect_running && !connected) {
        std::thread rThread(do_reconnect, userdata);
        rThread.detach();
    }
}


void context_state_callback(pa_context *c, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);

    //g_assert(c);
    mlog("enter:%s state:%d", __FUNCTION__, pa_context_get_state(c));
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY: {
            pa_operation *o;

            reconnect_timeout = 1;
            connected = true;

            pa_context_set_subscribe_callback(c, subscribe_cb, w);

            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                            PA_SUBSCRIPTION_MASK_SERVER|
                                            PA_SUBSCRIPTION_MASK_CARD), NULL, NULL))) {
                log("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);

            /* Keep track of the outstanding callbacks for UI tweaks */
            n_outstanding = 0;

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                log("pa_context_get_server_info() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_card_info_list(c, card_cb, w))) {
                log("pa_context_get_card_info_list() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                log("pa_context_get_sink_info_list() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                log("pa_context_get_source_info_list() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                log("pa_context_get_sink_input_info_list() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                log("pa_context_get_source_output_info_list() failed");
                return;
            }
            pa_operation_unref(o);
            n_outstanding++;

            break;
        }

        case PA_CONTEXT_FAILED: {
            connected = false;
            w->removeAll();
            pa_context_unref(context);
            context = NULL;

            connect_to_pulse(w);

            return;
        }

        case PA_CONTEXT_TERMINATED:
        default:
            return;
    }
}

pa_context* get_context(void) {
  return context;
}

bool connect_to_pulse(void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);

    if (context)
        return false;
    mlog("enter:%s", __FUNCTION__);
    pa_proplist *proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, "PulseAudio Volume Control");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, "org.PulseAudio.pavucontrol");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "audio-card");

    context = pa_context_new_with_proplist(api, NULL, proplist);
    //g_assert(context);

    pa_proplist_free(proplist);

    pa_context_set_state_callback(context, context_state_callback, w);

    //w->setConnectingMessage();
    if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL, NULL) < 0) {
        if (pa_context_errno(context) == PA_ERR_INVALID) {
            log("Connection to PulseAudio failed. Automatic retry in 5s");
            reconnect_timeout = 5;
        }
        else {
            if(!retry) {
                reconnect_timeout = -1;
            } else {
                log("Connection failed, attempting reconnect");
                reconnect_timeout = 5;
                sleep(reconnect_timeout);
                connect_to_pulse(w);
            }
        }
    }

    return false;
}

//主函数入口
int main(int argc, char *argv[]) {

    signal(SIGPIPE, SIG_IGN);

    AudioCore *ac = new AudioCore();

    pa_mainloop *m = pa_mainloop_new();
    //g_assert(m);
    api = pa_mainloop_get_api(m);
    //g_assert(api);

    connect_to_pulse(ac);
    int ret;
    if (reconnect_timeout >= 0)
        pa_mainloop_run(m, &ret);

    if (reconnect_timeout < 0)
        log("Fatal Error: Unable to connect to PulseAudio");

    delete ac;

    if (context)
        pa_context_unref(context);
    pa_mainloop_free(m);

    return 0;
}


int AudioManager::startPaService() {
    //1. modprobe module
    //2. systemctl start service
}

int AudioManager::stopPaService() {
    //1. systemctl stop service
    //2. modprobe -r module
}

int AudioManager::connectPaService() {

    AudioCore *ac = new AudioCore();

    pa_mainloop *m = pa_mainloop_new();
    //g_assert(m);
    api = pa_mainloop_get_api(m);
    //g_assert(api);

    connect_to_pulse(ac);
}




















