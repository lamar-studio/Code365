
#include "source.h"
#include "sourceoutput.h"

Source::Source() {

}

Source::~Source() {

}

int Source::updateVolume(std::string name, int percentage) {
    pa_operation* o;
    pa_volume_t vol = math_util::percentage_to_value<pa_volume_t>(percentage, PA_VOLUME_MUTED, PA_VOLUME_NORM);
    pa_cvolume_scale(&volume, vol);

    if (!(o = pa_context_set_source_volume_by_name(AudioCore::getInstance()->context, name.c_str(), &volume, NULL, NULL))) {
        rjlog_error("pa_context_set_source_volume_by_index() failed");
        return -1;
    }
    pa_operation_unref(o);

    return 0;
}

void Source::autoDefault(AudioCore *ac) {
    std::string hdmi_name, ana_name, def_name;

    for (std::map<uint32_t, Source*>::iterator i = ac->sources.begin(); i != ac->sources.end(); ++i) {
        Source *w = i->second;
        if (!w) {
            continue;
        }

        if (strstr(w->prio_type.c_str(), "USB") != NULL || strstr(w->prio_type.c_str(), "usb") != NULL) {
            def_name = w->name;
            break;
        } else if (strstr(w->prio_type.c_str(), "HDMI") != NULL || strstr(w->prio_type.c_str(), "hdmi") != NULL) {
            // hdmi nonsupport the source channel
            /*
            if (hdmi_name.empty()) {
                hdmi_name = w->name;
            }
            */
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

    if (def_name.empty()) {
        rjlog_warn("the default device is empty");
        return;
    }

    rjlog_info("[Source]:%s", def_name.c_str());
    if (def_name == ac->defaultSourceName) {
        rjlog_info("the same as the defaultSourceName");
        return;
    }

    updateDefault(def_name.c_str());
    ac->defaultSourceName = def_name;

    //move sourceOutput to default
    for (std::map<uint32_t, SourceOutput*>::iterator it = ac->sourceOutputs.begin();
         it != ac->sourceOutputs.end(); ++it) {
        it->second->moveSourceOutput(def_name.c_str());
    }

}

void Source::updateDefault(const char *name) {
    pa_operation* o;

    if (!(o = pa_context_set_default_source(AudioCore::getInstance()->context, name, NULL, NULL))) {
        rjlog_error("pa_context_set_default_source() failed");
        return;
    }
    pa_operation_unref(o);
}

