
#ifndef __SOUND_IF_H__
#define __SOUND_IF_H__

#include "alsa/asoundlib.h"

#define SUCCESS               (0)
#define FAIL                  (1)

typedef struct tagSndInfo {
    int samplearate;
    int channels;
    int format;
    int period_size;           //一个周期的帧大小

    int direction;             //pcm方向(播放 或者 录音)
    char *card;                //pcm的设备名,可以参考aplay的-D参数

    snd_pcm_t *pcm;            //pcm句柄

    int status;
} SND_INFO_T;


int sound_init();

int sound_open(SND_INFO_T *info);
int sound_close(SND_INFO_T *info);

int sound_playback(SND_INFO_T *playback);
int sound_capture(SND_INFO_T *capture);

void SetAlsaMasterVolume(long volume);

//int sound_set_volume();
//int sound_get_volume();

void pcm_config_list();
void ctl_config_list();



#endif



