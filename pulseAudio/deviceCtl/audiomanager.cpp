
#include "audiocore.h"
#include "audiomanager.h"

AudioManager::AudioManager() {

}

int AudioManager::startPaService() {
    return (AudioCore::getInstance()->paStart() == true) ? 0 : -1;
}

int AudioManager::stopPaService() {
    return (AudioCore::getInstance()->paStop() == true) ? 0 : -1;
}

//just test
void AudioManager::print() {
    AudioCore::getInstance()->printAll();
}





