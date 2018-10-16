
#ifndef audiocore_h
#define audiocore_h

#include <string>
#include <cstring>
#include <map>
#include <libintl.h>
#include <unistd.h>
#include <pulse/pulseaudio.h>

#include "net_thread.h"

#define DEBUG     (1)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)

class Sink;
class Source;
class SinkInput;
class SourceOutput;

class AudioCore {
public:
    AudioCore();
    virtual ~AudioCore();
    static AudioCore* getInstance();
    static void *do_reconnect(void *arg);
    static void reconnect(void *userdata);
    static void *main_loop(void *arg);
    static void sink_cb (
        pa_context *c,
        const pa_sink_info *i,
        int eol,
        void *userdata
        );
    static void sink_input_cb (
        pa_context *c,
        const pa_sink_input_info *i,
        int eol,
        void *userdata
        );
    static void source_output_cb (
        pa_context *c,
        const pa_source_output_info *i,
        int eol,
        void *userdata
        );
    static void source_cb (
        pa_context *c,
        const pa_source_info *i,
        int eol,
        void *userdata
        );
    static void server_info_cb (
        pa_context *c,
        const pa_server_info *i,
        void *userdata
        );
    static void subscribe_cb (
        pa_context *c,
        pa_subscription_event_type_t t,
        uint32_t index,
        void *userdata
        );
    static void context_state_callback (
        pa_context *c,
        void *userdata
        );

    bool paStart();
    bool paStop();
    bool paConnect(void *userdata);

    void updateSink(const pa_sink_info &info);
    void updateSource(const pa_source_info &info);
    void updateSinkInput(const pa_sink_input_info &info);
    void updateSourceOutput(const pa_source_output_info &info);
    void updateServer(const pa_server_info &info);

    void removeSink(uint32_t index);
    void removeSource(uint32_t index);
    void removeSinkInput(uint32_t index);
    void removeSourceOutput(uint32_t index);
    void removeAll();
    void printAll();

    int setSinkVolume(pa_volume_t vol);
    int setSourceVolume(pa_volume_t vol);

    std::map<uint32_t, Sink*, std::greater<uint32_t>> sinks;
    std::map<uint32_t, Source*, std::greater<uint32_t>> sources;
    std::map<uint32_t, SinkInput*, std::greater<uint32_t>> sinkInputs;
    std::map<uint32_t, SourceOutput*, std::greater<uint32_t>> sourceOutputs;

    pa_context *context;
    pa_mainloop_api *api;
    pa_mainloop *m;
    int retry;
    int retval;
    bool reconnect_running;
    bool connected;

private:
    class GC {
    public:
        ~GC() {
            if (mInstance != NULL) {
                delete mInstance;
                mInstance = NULL;
            }
        }
    };

    static void init();

    std::string defaultSinkName, defaultSourceName;
    uint32_t defaultSinkIdx, defaultSourceIdx;
    static pthread_once_t mOnce;
    static AudioCore *mInstance;
    static GC mGc;
};

#endif
