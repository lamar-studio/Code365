/*
 * Create by LaMar at 2018/09/05
 */
#ifndef __HWCONFIG_H
#define __HWCONFIG_H

#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<cstdlib>


#define SUCCESS   (0)
#define FAIL      (-1)

#define HWConfig_DEBUG 1
#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(HWConfig_DEBUG) printf(format"\n", ##args);    \
        } while(0)

using namespace std;

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

    vector<CFG_INFO_T> cfgInfos;
private:
    void loadHwConfigInfo();
};

#endif // __HWCONFIG_H




