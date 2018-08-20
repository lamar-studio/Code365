
#ifndef _SOUND_TEST_H_
#define _SOUND_TEST_H_

#include "alsa/asoundlib.h"

#define SUCCESS               (0)
#define FAIL                  (1)

typedef struct tagSndInfo {
    int samplearate;
    int channels;
    int format;
    int period_size;

    int direction;
    char *card;

    snd_pcm_t *pcm;

    int status;
} SND_INFO_T;


int sound_init();

int sound_open(SND_INFO_T *info);
int sound_close(SND_INFO_T *info);

int sound_playback(SND_INFO_T *playback);
int sound_capture(SND_INFO_T *capture);

int sound_set_volume();
int sound_get_volume();

void config_list();





#endif



