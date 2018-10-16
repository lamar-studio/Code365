
#include "audiocore.h"
#include "audiomanager.h"


int main(int argc, char *argv[]) {
    log("---test start---");
    //signal(SIGPIPE, SIG_IGN);
    int ret = 0;

    AudioManager *am = new AudioManager();

    ret |= am->startPaService();
    sleep(1000);
    am->print();
    ret |= am->stopPaService();
    sleep(1);
    log("---test end---ret:%d", ret);

    delete am;

    return 0;
}


























