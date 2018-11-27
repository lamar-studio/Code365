#ifndef _DEV_API_H
#define _DEV_API_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace rcdev
{

#define DEV_RES_BEST 0x1
#define DEV_RES_CUR  0x2


enum DevJsonHandle
{
    DEV_HANDLE_REQUEST_SND_DEV_LIST      = 101,
    DEV_HANDLE_SWITCH_SND_DEV            = 102,
    DEV_HANDLE_REQUEST_BACKLIGHT         = 111,
    DEV_HANDLE_SET_BACKLIGHT             = 112,
    DEV_HANDLE_SAVE_BACKLIGHT            = 113,

    DEV_HANDLE_REQUEST_SND_DEV_LIST_RET  = 201,
    DEV_HANDLE_SEND_SND_DEV_LIST         = 202,
    DEV_HANDLE_SWITCH_SND_DEV_RET        = 203,
    DEV_HANDLE_REQUEST_BACKLIGHT_RET     = 211,
    DEV_HANDLE_SET_BACKLIGHT_RET         = 212,
};

enum DevEventId
{
    DEV_EVENT_SAVE_BACKLIGHT             = 1,
};

typedef struct _DevMsg
{
    int msg_id;
    string msg;
} DevMsg;

typedef struct _DevResInfo {
    string res;
    int flag;
} DevResInfo;

//function declaration
typedef void (*onDevEvent)(DevMsg &msg, void *user);
typedef void (*onJsonEvent)(string &msg, void *user);

class AudioManager
{
public:
    AudioManager();
    AudioManager(const AudioManager& );
    AudioManager& operator=(const AudioManager& );
    ~AudioManager();

    void registerCallback(onDevEvent cb, onJsonEvent jcb, void* user);
    void doJsonCommand(int handle, const string& json);

    int startPaService(const char *type);
    int stopPaService();
    int setVolume(int volume);
    int getVolume();
    int changeHDMI(const char *profilename);
    int getHDMI();

private:
    void doEvent(DevMsg& msg);
    void doJsonEvent(string& json);

    onDevEvent    mCb;
    onJsonEvent   mJcb;
    void*         mUser;
};

class VideoManager
{
public:
    VideoManager();
    VideoManager(const VideoManager&);
    VideoManager& operator=(const VideoManager&);
    ~VideoManager();

    void registerCallback(onDevEvent cb, onJsonEvent jcb, void* user);
    void doJsonCommand(int handle, const string& json);

    int getBrightness();
    int setBrightness(int brightness);
    int getResolutionList(vector<DevResInfo>& resList);
    string getSupportResolution();
    string getCurrentResolution();
    string getOptimumResolution();
    int setResolution(string res);

private:
    void doEvent(DevMsg& msg);
    void doJsonEvent(string& json);

    onDevEvent    mCb;
    onJsonEvent   mJcb;
    void*         mUser;
};

} //namespace

#endif //_DEV_API_H
