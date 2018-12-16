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

    cout<<"end of the test!"<<endl;
    return 0;
}
















