/*
 * Create by LaMar at 2018/09/05
 */
#include <iostream>
#include <unistd.h>

#include "HWConfig.h"
#include "SoundTest.h"

static SoundTest st;

int main()
{
    unsigned long cnt = 0;

    while(1) {
        cnt++;
        log("=== test cnt:%ld ===", cnt);
        st.startRecord();
        sleep(3);
        st.stopRecord();
        sleep(1);
        st.startPlayback();
        sleep(3);
        st.stopPlayback();
        sleep(3);
    }
#if 0
    cout<<"wait 5s..."<<endl;
    sleep(5);
    cout<<"record start..."<<endl;
    st.startRecord();
    sleep(3);
    st.stopRecord();
    sleep(1);
    cout<<"playback start..."<<endl;
    st.startPlayback();
    sleep(3);
    st.stopPlayback();
    sleep(1);

    cout<<"wait 5s..."<<endl;
    sleep(5);
    cout<<"record start..."<<endl;
    st.startRecord();
    sleep(3);
    st.stopRecord();
    sleep(1);
    cout<<"playback start..."<<endl;
    st.startPlayback();
    sleep(3);
    st.stopPlayback();
    sleep(1);

    cout<<"wait 5s..."<<endl;
    sleep(5);
    cout<<"record start..."<<endl;
    st.startRecord();
    sleep(3);
    st.stopRecord();
    sleep(1);
    cout<<"playback start..."<<endl;
    st.startPlayback();
    sleep(3);
    st.stopPlayback();
    sleep(1);
#endif

    cout<<"end of the test!"<<endl;
    return 0;
}
















