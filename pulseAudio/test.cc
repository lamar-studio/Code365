
#include "audiomanager.h"


int main(int argc, char *argv[]) {

    //signal(SIGPIPE, SIG_IGN);
    int ret = 0;

    log("---test start---1");
    ret = AudioManager::getInstance()->startPaService();
    log("---test start---2");
    sleep(2);
    AudioManager::getInstance()->printDeviceList();
    sleep(2);
    ret = AudioManager::getInstance()->stopPaService();
    sleep(2);
    log("---test end---ret:%d", ret);


#if 0
    AudioManager *am = new AudioManager();

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
#endif

    return 0;
}


























