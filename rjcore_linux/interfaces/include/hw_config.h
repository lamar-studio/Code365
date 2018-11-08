/*
 * Create by LaMar at 2018/09/05
 */
#ifndef __HWCONFIG_H
#define __HWCONFIG_H

#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stdio.h>


typedef struct tagCfgInfo {
    string key;
    string val;
} CFG_INFO_T;

class HWConfig
{
public:
    HWConfig();
    ~HWConfig();

    /***********getter*************/
    bool getBoolValue(const char *key);
    int getIntValue(const char *key);
    float getFloatValue(const char *key);
    const char* getStringValue(const char *key);

    /******* for test only *******/
    void printAll();
private:
    void loadHwConfigInfo();
};

#endif // __HWCONFIG_H





