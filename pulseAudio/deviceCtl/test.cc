
#include "audiomanager.h"


int main(int argc, char *argv[]) {
    log("---test start---");
    //signal(SIGPIPE, SIG_IGN);
    int ret = 0;

    AudioManager *am = new AudioManager();

    ret = am->startPaService();
    log("---test start---2");
    sleep(1000);
    ret = am->stopPaService();
    sleep(2);
    log("---test end---ret:%d", ret);

    return 0;
}


























