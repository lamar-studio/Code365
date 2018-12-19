/*
 * Create by LaMar at 2018/09/05
 */
#ifndef __SOUND_TEST_H
#define __SOUND_TEST_H

using namespace std;

class SoundTest
{
public:
    SoundTest();
    ~SoundTest();

    bool initVolume();
    bool deInit();

    bool startPlayback();
    bool stopPlayback();

    bool startRecord();
    bool stopRecord();
};

#endif // __SOUND_TEST_H













