
#include "audiocore.h"
#include "sink.h"
#include "source.h"
#include "sinkinput.h"
#include "sourceoutput.h"

AudioCore*      AudioCore::mInstance = NULL;
AudioCore::GC   AudioCore::mGc;
pthread_once_t  AudioCore::mOnce;

AudioCore::AudioCore()
    : context(NULL),
      api(NULL),
      m(NULL),
      retry(3),
      retval(0),
      defaultSinkIdx(0),
      defaultSourceIdx(0) {

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

        log("Sink callback failure");
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

        log("Sink input callback failure");
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

        log("Source output callback failure");
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

        log("Source callback failure");
        return;
    }

    if (eol > 0) {
        return;
    }

    if (i->monitor_of_sink != PA_INVALID_INDEX)
        return;

    w->updateSource(*i);
}

void AudioCore::server_info_cb(pa_context *, const pa_server_info *i, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    if (!i) {
        log("Server info callback failure");
        return;
    }

    w->updateServer(*i);
}

void AudioCore::subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    mlog("[linzr]enter:%s case:%#x", __FUNCTION__, t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK);
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:     //0x0000U
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

        case PA_SUBSCRIPTION_EVENT_SOURCE:     //0x0001U
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

        case PA_SUBSCRIPTION_EVENT_SINK_INPUT:     //0x0002U
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

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:     //0x0003U
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

        case PA_SUBSCRIPTION_EVENT_SERVER: {     //0x0007U
                pa_operation *o;
                if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                    log("pa_context_get_server_info() failed");
                    return;
                }
                pa_operation_unref(o);
            }
            break;
    }
}

void AudioCore::context_state_callback(pa_context *c, void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);

    mlog("enter:%s state:%d", __FUNCTION__, pa_context_get_state(c));
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY: {
            pa_operation *o;

            pa_context_set_subscribe_callback(c, subscribe_cb, w);
            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT), NULL, NULL))) {
                log("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_server_info(c, server_info_cb, w))) {
                log("pa_context_get_server_info() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_info_list(c, sink_cb, w))) {
                log("pa_context_get_sink_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_source_info_list(c, source_cb, w))) {
                log("pa_context_get_source_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_sink_input_info_list(c, sink_input_cb, w))) {
                log("pa_context_get_sink_input_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            if (!(o = pa_context_get_source_output_info_list(c, source_output_cb, w))) {
                log("pa_context_get_source_output_info_list() failed");
                return;
            }
            pa_operation_unref(o);

            break;
        }

        //the pulseaudio interrupt.
        case PA_CONTEXT_FAILED: {
            w->removeAll();
            pa_context_unref(w->context);
            w->context = NULL;
            w->paConnect(w);
            break;
        }

        case PA_CONTEXT_TERMINATED:
        default:
            pa_context_unref(w->context);
            w->context = NULL;
            return;
    }
}

bool AudioCore::paConnect(void *userdata) {
    AudioCore *w = static_cast<AudioCore*>(userdata);
    log("enter paConnect w:%d instance:%d", w->retval, mInstance->retval);

    if (w->context)
        return false;

    w->context = pa_context_new(w->api, NULL);
    if (!w->context) {
        log("pa_context_new fail");
        return false;
    }

    pa_context_set_state_callback(w->context, context_state_callback, w);
    if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL, NULL) < 0) {
        if(!w->retry) {
            log("Connection to PulseAudio failed. Automatic retry in 5s");
            sleep(5);
            w->paConnect(w);
            w->retry--;
        } else {
            log("unable connect to PulseAudio exit.");
            pa_context_unref(context);
            return false;
        }
    }
    return true;
}

void* AudioCore::main_loop(void *arg) {
    AudioCore *w = static_cast<AudioCore*>(arg);
    pthread_detach(pthread_self());

    w->m = pa_mainloop_new();
    if (!(w->m)) {
        log("pa_mainloop_new fail:%s", strerror(errno));
        goto out;
    }

    w->api = pa_mainloop_get_api(w->m);
    if(!(w->api)) {
        log("pa_mainloop_get_api fail:%s", strerror(errno));
        goto out;
    }

    if (w->paConnect(w) == false) {
        log("connect_to_pulse fail:%s", strerror(errno));
        goto out;
    }

    if (pa_mainloop_run(w->m, &(w->retval)) < 0) {
        log("pa_mainloop_run fail:%s", strerror(errno));
        goto out;
    }

out:
    if (w->m)
        pa_mainloop_free(w->m);

    return NULL;
}

bool AudioCore::paStart() {
    pthread_t pid_t;

    if (pthread_create(&pid_t, NULL, main_loop, this) != 0) {
        log("pthread_create err:%s", strerror(errno));
        return false;
    }

    return true;
}

bool AudioCore::paStop() {

    if (context) {
        pa_context_disconnect(context);
        pa_mainloop_quit(m, retval);
    }

    return true;
}

void AudioCore::updateSink(const pa_sink_info &info) {
    Sink *w;
    mlog("[%s]name:%s index:%d", __FUNCTION__, info.name, info.index);

    if (sinks.count(info.index))
        w = sinks[info.index];
    else {
        sinks[info.index] = w = new Sink();
        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->autoDefault(this);

    return;
}

void AudioCore::updateSource(const pa_source_info &info) {
    Source *w;
    mlog("[%s]name:%s desc:%s", __FUNCTION__, info.name, info.description);

    if (sources.count(info.index))
        w = sources[info.index];
    else {
        sources[info.index] = w = new Source();

        w->index = info.index;
        w->prio_type = pa_proplist_gets(info.proplist, "alsa.name");
    }

    w->card_index = info.card;
    w->name = info.name;
    w->description = info.description;
    w->autoDefault(this);

    return;
}

void AudioCore::updateSinkInput(const pa_sink_input_info &info) {
    SinkInput *w;
    mlog("[%s]name:%s", __FUNCTION__, info.name);

    if (sinkInputs.count(info.index)) {
        w = sinkInputs[info.index];
    } else {
        sinkInputs[info.index] = w = new SinkInput();
        w->index = info.index;
        w->name = info.name;
    }

    return;
}

void AudioCore::updateSourceOutput(const pa_source_output_info &info) {
    SourceOutput *w;
    mlog("[%s]name:%s", __FUNCTION__, info.name);

    if (sourceOutputs.count(info.index))
        w = sourceOutputs[info.index];
    else {
        sourceOutputs[info.index] = w = new SourceOutput();

        w->index = info.index;
        w->name = info.name;
    }

    return;
}

void AudioCore::updateServer(const pa_server_info &info) {

    defaultSourceName = info.default_source_name ? info.default_source_name : "";
    defaultSinkName = info.default_sink_name ? info.default_sink_name : "";

    mlog("[%s]defSource:%s defSink:%s", __FUNCTION__, defaultSourceName.c_str(), defaultSinkName.c_str());
}

void AudioCore::removeSink(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sinks.count(index))
        return;

    delete sinks[index];
    sinks.erase(index);
}

void AudioCore::removeSource(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sources.count(index))
        return;

    delete sources[index];
    sources.erase(index);
}

void AudioCore::removeSinkInput(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sinkInputs.count(index))
        return;

    delete sinkInputs[index];
    sinkInputs.erase(index);
}

void AudioCore::removeSourceOutput(uint32_t index) {
    mlog("[%s]index:%d", __FUNCTION__, index);
    if (!sourceOutputs.count(index))
        return;

    delete sourceOutputs[index];
    sourceOutputs.erase(index);
}

void AudioCore::removeAll() {
    mlog("[%s]", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        removeSinkInput(it->first);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        removeSourceOutput(it->first);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        removeSink(it->first);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        removeSource(it->first);
}

void AudioCore::printAll() {
    mlog("[%s]", __FUNCTION__);
    for (std::map<uint32_t, SinkInput*>::iterator it = sinkInputs.begin(); it != sinkInputs.end(); ++it)
        mlog("sinkInput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, SourceOutput*>::iterator it = sourceOutputs.begin(); it != sourceOutputs.end(); ++it)
        mlog("sourceOutput:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Sink*>::iterator it = sinks.begin(); it != sinks.end(); ++it)
        mlog("sink:%s (%d)", it->second->name.c_str(), it->second->index);
    for (std::map<uint32_t, Source*>::iterator it = sources.begin(); it != sources.end(); ++it)
        mlog("source:%s (%d)", it->second->name.c_str(), it->second->index);
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


