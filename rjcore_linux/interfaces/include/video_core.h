#ifndef _VIDEO_CORE_H_
#define _VIDEO_CORE_H_

#include <string>
#include <vector>

using namespace std;

namespace rcdev
{

#define IS_SUPPORTED_RES(w, h) ((w) >= 1024 && (h) >= 720 && ((w) * (h) >= 1024 * 768))


typedef struct _RateInfo {
    string rate;
    int flag;
} RateInfo;

typedef struct _ResInfo {
    string res;
    int flag;
    vector<RateInfo> rateList;
} ResInfo;

typedef struct _DevInfo {
    string devType;
    vector<ResInfo> resList;
} DevInfo;


class VideoCore {
public:
    ~VideoCore();
    static VideoCore* getInstance();

    int getResolutionList(vector<DevInfo>& devList);
    int setResolution(string res);
    int getBrightness();
    int setBrightness(int brightness);

private:
    VideoCore();
    VideoCore(const VideoCore& );
    VideoCore& operator=(const VideoCore& );
    static VideoCore* mInstance;
    static pthread_once_t mOnce;

    class VideoCoreGarbo
    {
    public:
        ~VideoCoreGarbo()
        {
            if (VideoCore::mInstance) {
                delete VideoCore::mInstance;
                VideoCore::mInstance = NULL;
            }
        }
    };
    static VideoCoreGarbo Garbo;
    static void init();

    void clearRateInfo(RateInfo& rateInfo);
    void clearResInfo(ResInfo& resInfo);
    void clearDevInfo(DevInfo& devInfo);
};

} // namespace rcdev

#endif
