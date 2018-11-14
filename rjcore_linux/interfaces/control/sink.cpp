

#include "sink.h"
#include "sinkinput.h"

Sink::Sink() {

}

Sink::~Sink() {

}

int Sink::setVolume(std::string name, int percentage) {
    pa_operation *o;
    pa_volume_t vol = math_util::percentage_to_value<pa_volume_t>(percentage, PA_VOLUME_MUTED, PA_VOLUME_NORM);
    pa_cvolume_scale(&volume, vol);

    if (!(o = pa_context_set_sink_volume_by_name(AudioCore::getInstance()->context, name.c_str(), &volume, NULL, NULL))) {
        rjlog_error("pa_context_set_sink_volume_by_name() failed");
        return -1;
    }
    pa_operation_unref(o);

    return 0;
}

void Sink::autoDefault(AudioCore *ac) {
    std::string hdmi_name, ana_name, def_name;

    for (std::map<uint32_t, Sink*>::iterator i = ac->sinks.begin(); i != ac->sinks.end(); ++i) {
        Sink *w = i->second;
        if (!w) {
            continue;
        }

        if (strstr(w->prio_type.c_str(), "USB") != NULL || strstr(w->prio_type.c_str(), "usb") != NULL) {
            def_name = w->name;
            break;
        } else if (strstr(w->prio_type.c_str(), "HDMI") != NULL || strstr(w->prio_type.c_str(), "hdmi") != NULL) {
            if (hdmi_name.empty()) {
                hdmi_name = w->name;
            }
            continue;
        } else {
            if (ana_name.empty()) {
                ana_name = w->name;
            }
            continue;
        }
    }

    if (def_name.empty()) {
        def_name = hdmi_name.empty() ? ana_name : hdmi_name;
    }

    rjlog_info("[Sink]:%s", def_name.c_str());
    ac->updateSinkVolume();
    if (def_name == ac->defaultSinkName) {
        rjlog_info("the same as defaultSinkName");
        return;
    }

    updateDefault(def_name.c_str());
    ac->defaultSinkName = def_name;

    //move sinkInput to default
    for (std::map<uint32_t, SinkInput*>::iterator it = ac->sinkInputs.begin(); it != ac->sinkInputs.end(); ++it)
        it->second->moveSinkInput(def_name.c_str());

}

void Sink::updateDefault(const char *name) {
    pa_operation *o;

    if (!(o = pa_context_set_default_sink(AudioCore::getInstance()->context, name, NULL, NULL))) {
        rjlog_error("pa_context_set_default_sink() failed");
        return;
    }
    pa_operation_unref(o);
}


