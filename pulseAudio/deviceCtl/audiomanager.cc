
#include "audiocore.h"
#include "audiomanager.h"

AudioManager::AudioManager() {

}

int AudioManager::startPaService() {
    //1.modprobe module
    //system("systemctl start pulseaudio-rcd.service");
    //system("pulseaudio --start --log-target=syslog");
    return (AudioCore::getInstance()->paStart() == true) ? 0 : -1;
}

int AudioManager::stopPaService() {

    int ret = (AudioCore::getInstance()->paStop() == true) ? 0 : -1;
    //system("systemctl stop pulseaudio-rcd.service");
    //system("pulseaudio -k");
    //2. modprobe -r module
    return ret;
}

//just test
void AudioManager::print() {
    AudioCore::getInstance()->printAll();
}





