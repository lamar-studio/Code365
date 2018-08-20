
#include <stdio.h>

#include "sound_if.h"

#define ALSA_LIB_VERSION     "test alsa-lib ver 1.1.6"

extern SND_INFO_T *g_record_info;
extern SND_INFO_T *g_playback_info;

int main()
{
    printf("start the program:\n");
    printf("INFO:%s\n", ALSA_LIB_VERSION);

    sound_init();
    sound_playback(g_playback_info);
    sound_close(g_playback_info);

    printf("exit the program.\n");

    return 0;
}



