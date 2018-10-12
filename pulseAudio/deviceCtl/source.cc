
#include "source.h"
#include "sourceoutput.h"

Source::Source() {

}

void Source::updateVolume(pa_volume_t v) {

    updateChannelVolume(0, v, true);

    pa_operation* o;

    if (!(o = pa_context_set_source_volume_by_index(AudioCore::getInstance()->context, index, &volume, NULL, NULL))) {
        log("pa_context_set_source_volume_by_index() failed");
        return;
    }

    pa_operation_unref(o);
}

void Source::autoDefault(AudioCore *ac) {

    std::string usb_name;
    std::string hdmi_name;
    std::string ana_name;
    std::string def_name;

    uint32_t idx = 0;

    for (std::map<uint32_t, Source*>::iterator i = ac->sources.begin(); i != ac->sources.end(); ++i) {
        Source *w = i->second;
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

    mlog("[Source]autoDefault:%s",def_name.c_str());
    updateDefault(def_name.c_str());
    //move sourceOutput to default
    for (std::map<uint32_t, SourceOutput*>::iterator it = ac->sourceOutputs.begin(); it != ac->sourceOutputs.end(); ++it)
        it->second->moveSourceOutput(def_name.c_str());

}

void Source::updateDefault(const char *name) {
    pa_operation* o;

    if (!(o = pa_context_set_default_source(AudioCore::getInstance()->context, name, NULL, NULL))) {
        log("pa_context_set_default_source() failed");
        return;
    }
    pa_operation_unref(o);
}

