/*
 * Create by LaMar at 2018/09/05
 */
#ifndef __SOUND_TEST_H
#define __SOUND_TEST_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <alsa/asoundlib.h>

using namespace std;

typedef struct tagSndInfo {
    int samplearate;
    int channels;
    int period_size;
    snd_pcm_format_t format;

    int direction;
    char* card;

    snd_pcm_t* pcm;

    int status;
} SND_INFO_T;


class SoundTest
{
public:
    SoundTest();
    ~SoundTest();

    bool init();

    int openSoundCard(SND_INFO_T *info);
    void closeSoundCard(SND_INFO_T *info);

    bool startPlayback();
    bool stopPlayback();

    bool startRecord();
    bool stopRecord();

    void playbackLoop(void *arg);
    void recordLoop(void *arg);

    void testAll();
    void info();

private:
    void loadHwConfigInfo();
};

#endif // __SOUND_TEST_H













