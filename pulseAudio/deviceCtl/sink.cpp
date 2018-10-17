

#include "sink.h"
#include "sinkinput.h"

Sink::Sink() {

}

Sink::~Sink() {

}

void Sink::updateVolume(pa_volume_t v) {
    updateChannelVolume(0, v, true);
    pa_operation *o;

    if (!(o = pa_context_set_sink_volume_by_index(AudioCore::getInstance()->context, index, &volume, NULL, NULL))) {
        log("pa_context_set_sink_volume_by_index() failed");
        return;
    }
    pa_operation_unref(o);
}

void Sink::autoDefault(AudioCore *ac) {
    std::string hdmi_name;
    std::string ana_name;
    std::string def_name;

    for (std::map<uint32_t, Sink*>::iterator i = ac->sinks.begin(); i != ac->sinks.end(); ++i) {
        Sink *w = i->second;
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

    mlog("[Sink]autoDefault:%s", def_name.c_str());
    if (def_name == ac->defaultSinkName)
        return;

    updateDefault(def_name.c_str());
    ac->defaultSinkName = def_name;
    //move sinkInput to default
    for (std::map<uint32_t, SinkInput*>::iterator it = ac->sinkInputs.begin(); it != ac->sinkInputs.end(); ++it)
        it->second->moveSinkInput(def_name.c_str());

}

void Sink::updateDefault(const char *name) {
    pa_operation *o;

    if (!(o = pa_context_set_default_sink(AudioCore::getInstance()->context, name, NULL, NULL))) {
        log("pa_context_set_default_sink() failed");
        return;
    }
    pa_operation_unref(o);
}


