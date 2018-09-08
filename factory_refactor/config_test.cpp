/*
 * Create by LaMar at 2018/09/05
 */
#include <iostream>

#include "HWConfig.h"
#include "SoundTest.h"

int main()
{
    HWConfig *ini = new HWConfig();

    ini->printAll();

    int mem = ini->getIntValue("MEM");
    bool bWifi = ini->getBoolValue("WIFI");
    const char *str_Cpu = ini->getStringValue("CPU");

    cout<<"[MEM]-int:\t"<<mem<<endl;
    cout<<"[WIFI]-bool:\t"<<bWifi<<endl;
    cout<<"[CPU]-string:\t"<<str_Cpu<<endl;

    delete ini;

    SoundTest *sound = new SoundTest();



    return 0;
}
















