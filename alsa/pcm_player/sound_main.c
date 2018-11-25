
#include <stdio.h>

#include "sound_if.h"

#define ALSA_LIB_VERSION     "test alsa-lib ver 1.1.6"

extern SND_INFO_T *g_record_info;
extern SND_INFO_T *g_playback_info;

int main()
{
    printf("start the program:\n");
    printf("INFO:%s\n", ALSA_LIB_VERSION);
    printf("\n=============================================\n");

    ctl_config_list();
    pcm_config_list();

    printf("\n=============================================\n");
    printf("exit the program.\n");

    return 0;
}



