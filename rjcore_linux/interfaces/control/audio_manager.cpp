#include "dev_api.h"
#include "audiocore.h"

namespace rcdev
{

AudioManager::AudioManager()
    : mCb(NULL)
    , mJcb(NULL)
    , mUser(NULL)
{
}

AudioManager::~AudioManager()
{

}

void AudioManager::registerCallback(onDevEvent cb, onJsonEvent jcb, void* user)
{
    mCb = cb;
    mJcb = jcb;
    mUser = user;
}

void AudioManager::doEvent(DevMsg& msg)
{
    if (mCb == NULL) {
        rjlog_error("mCb is NULL!");
        return;
    }
    rjlog_info("send DevEvent, id: %d, msg: %s", msg.msg_id, msg.msg.c_str());
    mCb(msg, mUser);
}

void AudioManager::doJsonEvent(string& json)
{
    if (mJcb == NULL) {
        rjlog_error("mJcb is NULL!");
        return;
    }
    rjlog_info("send json: %s", json.c_str());
    mJcb(json, mUser);
}

void AudioManager::doJsonCommand(int handle, const string& json)
{
    string out_json;

    rjlog_info("recv json: %s", json.c_str());
    switch (handle) {
        case DEV_HANDLE_REQUEST_SND_DEV_LIST:
            out_json = "{\"handle\": 201}";
            doJsonEvent(out_json);
            break;
        case DEV_HANDLE_SWITCH_SND_DEV:
            out_json = "{\"handle\": 201}";
            doJsonEvent(out_json);
            break;
        default:
            break;
    }
}

int AudioManager::startPaService(const char *type)
{
    CHECK_FUNCTION_IN();
    return (AudioCore::getInstance()->paStart(type) == true) ? 0 : -1;
}
int AudioManager::stopPaService()
{
    CHECK_FUNCTION_IN();
    return (AudioCore::getInstance()->paStop() == true) ? 0 : -1;
}

int AudioManager::setVolume(int volume)
{
    return AudioCore::getInstance()->setSinkVolume(volume);
}

int AudioManager::getVolume()
{
    return AudioCore::getInstance()->getSinkVolume();
}

int AudioManager::changeHDMI(const char *profilename)
{
    return AudioCore::getInstance()->changeProfile(profilename);
}

int AudioManager::getHDMI()
{
    //AudioCore::getInstance()->getActiveProfile();
    return 0;
}


} //namespace
