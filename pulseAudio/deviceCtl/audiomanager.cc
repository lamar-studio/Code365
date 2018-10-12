
#include "audiomanager.h"

AudioManager::AudioManager() {

}

int AudioManager::startPaService() {
    //1.modprobe module
    //2.systemctl start service
    return (AudioCore::getInstance()->paStart() == true) ? 0 : -1;
}

int AudioManager::stopPaService() {
    return (AudioCore::getInstance()->paStop() == true) ? 0 : -1;
    //1. systemctl stop service
    //2. modprobe -r module
}

//just test
void AudioManager::print() {
    AudioCore::getInstance()->printAll();
}





