
#include "audiocore.h"
#include "audiomanager.h"


int main(int argc, char *argv[]) {
    log("---test start---");
    int ret = 0;

    AudioManager *am = new AudioManager();

    ret |= am->startPaService();
    while(1) {
        am->print();
        sleep(20);
    }
    ret |= am->stopPaService();
    sleep(1);
    log("---test end---ret:%d", ret);

    delete am;

    return 0;
}


























