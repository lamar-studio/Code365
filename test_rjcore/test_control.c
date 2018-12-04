

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


#include "sysrjcore_linux.h"


#define DEBUG     (0)
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)


int main()
{
    log("=======Test Start =======");
    unsigned int cnt = 0;
    int idx = 0;
    int lan_idx = 0;
    int vol = 100;
    int btness = 0;
    bool stat = false;
    const char *res[] = {"1920x1080", "1680x1050", "1400x1050", "1600x900", "1280x1024",
                   "1440x900", "1280x800", "1152x864", "1280x720", "1024x768"};
    const char *hn[] = {"RainOS", "LinOS", "RuijieOS", "Ubuntu", "GitHubOS",
                  "ArchLinux", "Win7", "WinXP", "Win10", "CentOS"};
    const char *lang[] = {"chinese", "english", "zhongwen", "yinwen"};
    char buf[128] = {0};
    Init("shine", NULL);
    sleep(1);

    while(1) {
        log("+_+:----~~~-----+_+ cnt:%d", cnt);
        /*
        usleep(500);
        setBrightness(btness);
        log("setBrightness:%d", btness);
        log("getBrightness:%d", getBrightness());
        btness = btness + 5;
        if (btness >= 100)
            btness = 0;
        */
        /*
        usleep(500);
        setVoiceVolume(vol);
        log("setVoiceVolume:%d", vol);
        log("getVoiceVolume:%d", getVoiceVolume());
        */
        usleep(500);
        setHdmiVoiceStatus(stat);
        log("setHdmiVoiceStatus:%d", stat);
        log("getHdmiVoiceStatus:%d", getHdmiVoiceStatus());
        stat = !stat;
        /*
        usleep(500);
        setPowerState(stat);
        log("setPowerState:%d", stat);
        log("getPowerState:%d", getPowerState());

        usleep(500);
        memset(buf, 0, sizeof(buf));
        getCurrentResolution(buf, sizeof(buf));
        log("getCurrentResolution:%s", buf);

        memset(buf, 0, sizeof(buf));
        getSupportResolution(buf, sizeof(buf));
        log("getSupportResolution:%s", buf);

        memset(buf, 0, sizeof(buf));
        getOptimumResolution(buf, sizeof(buf));
        log("getOptimumResolution:%s", buf);

        //setDeviceResolution(res[idx], 50);
        //log("setDeviceResolution:%s", res[idx]);
        usleep(500);
        memset(buf, 0, sizeof(buf));
        setHostname(hn[idx]);
        log("setHostname:%s", hn[idx]);
        getHostname(buf, sizeof(buf));
        log("getHostname:%s", buf);
        idx++;
        if (idx > 9)
            idx = 0;

        usleep(500);
        setLanguage(lang[lan_idx]);
        log("setLanguage:%s", lang[lan_idx]);
        system("cat /etc/environment | grep LANG");
        lan_idx++;
        if (lan_idx > 3)
            lan_idx = 0;
        */
        usleep(100*1000);
        cnt++;
    }

    log("=======Test End =======");
}

































