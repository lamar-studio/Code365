/*
 * Create by LaMar at 2018/09/05
 */
#ifndef __SOUND_TEST_H
#define __SOUND_TEST_H

#include <alsa/asoundlib.h>

using namespace std;

typedef struct tagSndInfo {
    int samplearate;
    int channels;
    int period_size;
    snd_pcm_format_t format;
    snd_pcm_stream_t direction;
    const char* card;

    snd_pcm_t* pcm;

    int status;
} SND_INFO_T;

typedef enum tagSndStatus {
    SOUND_RECORD_START = 0,
    SOUND_RECORD_STOP,
    SOUND_PLAYBACK_START,
    SOUND_PLAYBACK_STOP,
    SOUND_UNKNOW
} SND_STATUS_T;


class SoundTest
{
public:
    SoundTest();
    ~SoundTest();

    static SoundTest* getInstance();
    bool init();
    int openSoundCard(SND_INFO_T *info);
    void closeSoundCard(SND_INFO_T *info);

    bool startPlayback();
    bool stopPlayback();

    bool startRecord();
    bool stopRecord();

    void testAll();
    void info();

private:
    static SoundTest *mInstance;
};

#endif // __SOUND_TEST_H













