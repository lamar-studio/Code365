
#include "audiocore.h"
#include "sink.h"
#include "source.h"
#include "sinkinput.h"
#include "sourceoutput.h"

AudioCore*      AudioCore::mInstance = NULL;
AudioCore::GC   AudioCore::mGc;
pthread_once_t  AudioCore::mOnce;

AudioCore::AudioCore()
    : defaultSinkIdx(0),
      defaultSourceIdx(0),
      innerCardIdx(0),
      context(NULL),
      bHDMI(false),
      api(NULL),
      m(NULL),
      retry(3),
      retval(0),
      mInited(false) {

}

AudioCore::~AudioCore() {

}

AudioCore* AudioCore::getInstance() {
    pthread_once(&mOnce, init);
    return mInstance;
}

void AudioCore::init() {
    mInstance = new AudioCore();
}

void AudioCore::sink_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (eol < 0) {
        if (pa_context_errno(w->context) == PA_ERR_NOENTITY)
            return;

        rjlog_error("Sink callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateSink(*i);
}

void AudioCore::sink_input_cb(pa_context *, const pa_sink_input_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (eol < 0) {
        if (pa_context_errno(w->context) == PA_ERR_NOENTITY)
            return;

        rjlog_error("Sink input callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateSinkInput(*i);
}

void AudioCore::source_output_cb(pa_context *, const pa_source_output_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (eol < 0) {
        if (pa_context_errno(w->context) == PA_ERR_NOENTITY)
            return;

        rjlog_error("Source output callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->updateSourceOutput(*i);
}

void AudioCore::source_cb(pa_context *, const pa_source_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (eol < 0) {
        if (pa_context_errno(w->context) == PA_ERR_NOENTITY)
            return;

        rjlog_error("Source callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    if ((i->monitor_of_sink != PA_INVALID_INDEX) || !(i->flags & PA_SOURCE_HARDWARE))
        return;

    w->updateSource(*i);
}

void AudioCore::server_info_cb(pa_context *, const pa_server_info *i, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (!i) {
        rjlog_error("Server info callback failure");
        return;
    }

    if (strstr(i->default_source_name, "auto_null") != NULL || strstr(i->default_sink_name, "auto_null") != NULL) {
        rjlog_error("The system is in Abnormal status.(the snd driver may load fail)");
        w->paStop();
        return;
    }

    w->updateServer(*i);
    w->updateSinkVolume();
}

void AudioCore::subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    rjlog_info(" case:%#x, type:%#x", t&PA_SUBSCRIPTION_EVENT_FACILITY_MASK, t&PA_SUBSCRIPTION_EVENT_TYPE_MASK);
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:     //0x0000U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                w->removeSink(index);
                Sink::autoDefault(w);
            } else if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_NEW ||
                       (t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_CHANGE) {
                pa_operation *o;
                if (!(o = pa_context_get_sink_info_by_index(c, index, sink_cb, w))) {
                    rjlog_error("pa_context_get_sink_info_by_index() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE:     //0x0001U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                w->removeSource(index);
                Source::autoDefault(w);
            } else if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_NEW) {
                pa_operation *o;
                if (!(o = pa_context_get_source_info_by_index(c, index, source_cb, w))) {
                    rjlog_error("pa_context_get_source_info_by_index() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SINK_INPUT:     //0x0002U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                w->removeSinkInput(index);
            } else if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_NEW) {
                pa_operation *o;
                if (!(o = pa_context_get_sink_input_info(c, index, sink_input_cb, w))) {
                    rjlog_error("pa_context_get_sink_input_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:     //0x0003U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                w->removeSourceOutput(index);
            } else if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_NEW) {
                pa_operation *o;
                if (!(o = pa_context_get_source_output_info(c, index, source_output_cb, w))) {
                    rjlog_error("pa_context_get_sink_input_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;

        case PA_SUBSCRIPTION_EVENT_SERVER: {     //0x0007U
                pa_operation *o;
                if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                    rjlog_error("pa_context_get_server_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;
    }
}

void AudioCore::context_state_callback(pa_context *c, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);

    rjlog_info(" state:%d", pa_context_get_state(c));
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY: {
            pa_operation *o;
            w->mInited = true;
            w->bHDMI = getHdmiVoiceStatus();

            pa_context_set_subscribe_callback(c, subscribe_cb, w);
            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT), NULL, NULL))) {
                rjlog_error("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                rjlog_error("pa_context_get_server_info() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                rjlog_error("pa_context_get_sink_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                rjlog_error("pa_context_get_source_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                rjlog_error("pa_context_get_sink_input_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                rjlog_error("pa_context_get_source_output_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            break;
        }

        //the pulseaudio interrupt.
        case PA_CONTEXT_FAILED: {
            rjlog_error("reconnect the PulseAudio...");
            w->mInited = false;
            w->removeAll();
            if (w->context) {
                pa_context_unref(w->context);
                w->context = NULL;
            }
            w->paConnect(w);
            break;
        }

        case PA_CONTEXT_TERMINATED:
        default:
            w->mInited = false;
            w->removeAll();
            rjlog_info("exit the audio module.");
            return;
    }
}

void AudioCore::get_sink_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    AudioCore *w = static_cast<AudioCore *>(userdata);
    if (eol < 0) {
        if (pa_context_errno(w->context) == PA_ERR_NOENTITY)
            return;

        rjlog_error("get_sink_volume_callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    w->sinkVol = i->volume;

    //pa_threaded_mainloop_signal(This->m_mainloop, 0);
}

void AudioCore::updateSinkVolume() {
    pa_operation *o;

    rjlog_info(" updateSinkVolume ");
    if (!(o = pa_context_get_sink_info_by_name(context, defaultSinkName.c_str(), get_sink_volume_callback, this))) {
        rjlog_error("pa_context_set_default_sink() failed");
        return;
    }
    pa_operation_unref(o);
    //wait_loop(o, m_mainloop);
}

bool AudioCore::paConnect(void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);

    if (w->context) {
        return false;
    }

    pa_proplist *proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, "ruijie sound control");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, "ruijie.com.snd.ctl");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ICON_NAME, "audio-card");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_VERSION, "V1.0");

    w->context = pa_context_new_with_proplist(w->api, NULL, proplist);
    pa_proplist_free(proplist);
    if (!w->context) {
        rjlog_error("pa_context_new fail");
        return false;
    }

    pa_context_set_state_callback(w->context, context_state_callback, w);
    if (pa_context_connect(w->context, NULL, PA_CONTEXT_NOFAIL, NULL) < 0) {
        if(!w->retry) {
            rjlog_error("Connection to PulseAudio failed. Automatic retry in 5s");
            sleep(5);
            w->paConnect(w);
            w->retry--;
        } else {
            rjlog_error("Unable connect to PulseAudio exit!");
            pa_context_unref(w->context);
            w->context = NULL;
            return false;
        }
    }
    return true;
}

void* AudioCore::main_loop(void *arg) {
    AudioCore *w = static_cast<AudioCore*>(arg);
    pthread_detach(pthread_self());

#if 0
    //load the snd module
    if (rj_system("modprobe snd_hda_intel") < 0) {
        rjlog_error("load snd_hda_intel err:%s", strerror(errno));
        goto out;
    }

    //start the pulseaudio service
    if (rj_system("pulse-daemon &") < 0) {
        rjlog_error("start pulseaudio err:%s", strerror(errno));
        goto out;
    }
    usleep(5000);    //wait the service stable.
#endif

    //connect
    w->m = pa_mainloop_new();
    if (!(w->m)) {
        rjlog_error("pa_mainloop_new fail:%s", strerror(errno));
        goto out;
    }

    w->api = pa_mainloop_get_api(w->m);
    if(!(w->api)) {
        rjlog_error("pa_mainloop_get_api fail:%s", strerror(errno));
        goto out;
    }

    if (w->paConnect(w) == false) {
        rjlog_error("connect_to_pulse fail:%s", strerror(errno));
        goto out;
    }

    if (pa_mainloop_run(w->m, &(w->retval)) < 0) {
        rjlog_error("pa_mainloop_run fail:%s", strerror(errno));
        goto out;
    }

    rjlog_info("exit the mainloop.");
out:
    if (w->context) {
        pa_context_unref(w->context);
        w->context = NULL;
    }
    if (w->m) {
        pa_mainloop_free(w->m);
        w->m = NULL;
    }

    return NULL;
}

bool AudioCore::paStart(std::string type) {
    pthread_t pid_t;

    termialType = type;
    if (pthread_create(&pid_t, NULL, main_loop, this) != 0) {
        rjlog_error("pthread_create err:%s", strerror(errno));
        return false;
    }

    return true;
}

bool AudioCore::paStop() {
    bool ret = true;

    //disconnect
    if (context) {
        pa_context_disconnect(context);
        pa_mainloop_quit(m, retval);
    }

#if 0
    //stop the pulseaudio service
    if (rj_system("pkill pulse-daemon") < 0) {
        rjlog_error("stop pulseaudio err:%s", strerror(errno));
        ret = false;
    }
    if (rj_system("pkill pulseaudio") < 0) {
        rjlog_error("stop pulseaudio err:%s", strerror(errno));
        ret = false;
    }
    usleep(5000);    //wait the service complete quit.

    //unload the snd module
    if (rj_system("modprobe -r snd_hda_intel") < 0) {
        rjlog_error("unload snd_hda_intel err:%s", strerror(errno));
        ret = false;
    }
#endif

    return ret;
}

void AudioCore::updateSink(const pa_sink_info &info) {
    Sink *w;
    std::string vdiBuiltInUsb("pci-0000:00:14.0-usb-0:3:1.0");

    if (sinks.count(info.index)) {
        w = sinks[info.index];
    } else {
        sinks[info.index] = w = new Sink();
        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");

        //adapter to the VDI inner USB audio
        if (termialType.compare(RJ_VDI_TYPE) == 0) {
            if (strstr(w->prio_type.c_str(), "USB") != NULL || strstr(w->prio_type.c_str(), "usb") != NULL) {
                if (vdiBuiltInUsb == pa_proplist_gets(info.proplist, "device.bus_path")) {
                    w->prio_type = "inner analog";
                    innerCardIdx = info.card;
                    innerCardName = "VDI inner USB";
                }
            }
        } else if (termialType.compare(RJ_IDV_TYPE) == 0) {
            if (strstr(w->prio_type.c_str(), "USB") == NULL || strstr(w->prio_type.c_str(), "usb") == NULL) {
                w->prio_type = "inner analog";
                innerCardIdx = info.card;
                innerCardName = "IDV inner alc662";
            }
        } else {
            rjlog_warn("nonsupport this termial type:%s", termialType.c_str());
        }
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;

    rjlog_info(" prio_type:%s index:%d", w->prio_type.c_str(), info.index);
    Sink::autoDefault(this);

    return;
}

void AudioCore::updateSource(const pa_source_info &info) {
    Source *w;
    std::string vdiBuiltInUsb("pci-0000:00:14.0-usb-0:3:1.0");

    if (sources.count(info.index)) {
        w = sources[info.index];
    } else {
        sources[info.index] = w = new Source();

        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");

        //adapter to the VDI inner USB audio
        if (termialType.compare(RJ_VDI_TYPE) == 0) {
            if (strstr(w->prio_type.c_str(), "USB") != NULL || strstr(w->prio_type.c_str(), "usb") != NULL) {
                if (vdiBuiltInUsb == pa_proplist_gets(info.proplist, "device.bus_path")) {
                    w->prio_type = "inner analog";
                }
            }
        }

    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;

    rjlog_info(" prio_type:%s index:%d", w->prio_type.c_str(), info.index);
    Source::autoDefault(this);

    return;
}

void AudioCore::updateSinkInput(const pa_sink_input_info &info) {
    SinkInput *w;
    rjlog_info(" name:%s index:%d", info.name, info.index);

    if (sinkInputs.count(info.index)) {
        w = sinkInputs[info.index];
    } else {
        sinkInputs[info.index] = w = new SinkInput();
        w->index = info.index;
        w->name = info.name;
    }
    w->moveSinkInput(defaultSinkName.c_str());

    return;
}

void AudioCore::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutput *w;
    rjlog_info(" name:%s", info.name);

    if (sourceOutputs.count(info.index)) {
        w = sourceOutputs[info.index];
    } else {
        sourceOutputs[info.index] = w = new SourceOutput();

        w->index = info.index;
        w->name = info.name;
    }
    w->moveSourceOutput(defaultSourceName.c_str());

    return;
}

void AudioCore::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    rjlog_info(" defSource:%s defSink:%s", defaultSourceName.c_str(), defaultSinkName.c_str());
}

void AudioCore::removeSink(uint32_t index) {
    rjlog_info(" index:%d", index);
    if (!sinks.count(index)) {
        return;
    }

    delete sinks[index];
    sinks.erase(index);
}

void AudioCore::removeSource(uint32_t index) {
    rjlog_info(" index:%d", index);
    if (!sources.count(index)) {
        return;
    }

    delete sources[index];
    sources.erase(index);
}

void AudioCore::removeSinkInput(uint32_t index) {
    rjlog_info(" index:%d", index);
    if (!sinkInputs.count(index)) {
        return;
    }

    delete sinkInputs[index];
    sinkInputs.erase(index);
}

void AudioCore::removeSourceOutput(uint32_t index) {
    rjlog_info(" index:%d", index);
    if (!sourceOutputs.count(index)) {
        return;
    }

    delete sourceOutputs[index];
    sourceOutputs.erase(index);
}

void AudioCore::removeAll() {
    rjlog_info("remove all snd device");
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it) {
        removeSinkInput(it->first);
    }

    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it) {
        removeSourceOutput(it->first);
    }

    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it) {
        removeSink(it->first);
    }

    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it) {
        removeSource(it->first);
    }
}

void AudioCore::printAll() {
    rjlog_info("print all snd device");
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it) {
        rjlog_info("sinkInput:%s (%d)", it->second->name.c_str(), it->second->index);
    }

    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it) {
        rjlog_info("sourceOutput:%s (%d)", it->second->name.c_str(), it->second->index);
    }

    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it) {
        rjlog_info("sink:%s (%d)", it->second->name.c_str(), it->second->index);
    }

    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it) {
        rjlog_info("source:%s (%d)", it->second->name.c_str(), it->second->index);
    }
}

int AudioCore::setSinkVolume(int volume) {
    if (mInited == false) {
        rjlog_error("the audio module init fail");
        return -1;
    }
    pa_operation *o;
    pa_volume_t vol = math_util::percentage_to_value<pa_volume_t>(volume, PA_VOLUME_MUTED, PA_VOLUME_NORM);
    pa_cvolume_scale(&sinkVol, vol);
    rjlog_info("ui_vol:%d actual_val:%d", volume, vol);

    if (!(o = pa_context_set_sink_volume_by_name(context, defaultSinkName.c_str(), &sinkVol, NULL, NULL))) {
        rjlog_error("pa_context_set_sink_volume_by_name() failed");
        return -1;
    }
    pa_operation_unref(o);

    return 0;
}

int AudioCore::getSinkVolume() {
    if (mInited == false) {
        rjlog_error("the audio module init fail");
        return -1;
    }

    int vol = static_cast<int>(pa_cvolume_max(&sinkVol) * 100.0f / PA_VOLUME_NORM + 0.5f);
    rjlog_info("ui_vol:%d", vol);

    return vol;
}

int AudioCore::changeProfile(const char *profileName) {
    if (mInited == false) {
        rjlog_error("the audio module init fail");
        return -1;
    }

    if (innerCardName.empty()) {
        rjlog_error("not found the inner card");
        return -1;
    }

    if (strncmp(profileName, RJ_HDMI_AUDIO, strlen(RJ_HDMI_AUDIO)) == 0) {
        bHDMI = true;
        if (termialType.compare(RJ_IDV_TYPE) == 0) {
            pa_operation* o;

            if (!(o = pa_context_set_card_profile_by_index(context, innerCardIdx, HDMI_PROFILE, NULL, NULL))) {
                rjlog_error("pa_context_set_card_profile_by_index() failed");
                return -1;
            }
            pa_operation_unref(o);
        } else if (termialType.compare(RJ_VDI_TYPE) == 0) {
            Sink::autoDefault(this);
        } else {
            rjlog_warn("nonsupport this termial type");
            return -1;
        }
    } else if (strncmp(profileName, RJ_ANALOG_AUDIO, strlen(RJ_ANALOG_AUDIO)) == 0) {
        bHDMI = false;
        if (termialType.compare(RJ_IDV_TYPE) == 0) {
            pa_operation* o;

            if (!(o = pa_context_set_card_profile_by_index(context, innerCardIdx, ANA_PROFILE, NULL, NULL))) {
                rjlog_error("pa_context_set_card_profile_by_index() failed");
                return -1;
            }
            pa_operation_unref(o);
        } else if (termialType.compare(RJ_VDI_TYPE) == 0) {
            Sink::autoDefault(this);
        } else {
            rjlog_warn("nonsupport this termial type");
            return -1;
        }
    }

    rjlog_info("profileName:%s innerCard:%s", profileName, innerCardName.c_str());

    return 0;
}


