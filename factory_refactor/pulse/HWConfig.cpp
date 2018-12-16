/*
 * Create by LaMar at 2018/09/05
 */
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>

#include "HWConfig.h"

HWConfig::HWConfig()
{
    loadHwConfigInfo();
}

HWConfig::~HWConfig()
{

}

void HWConfig::loadHwConfigInfo()
{
    const char *info = "CPU:i5-6200U;MEM:8;USB:2/6;SSD:128+256;MEMC:32;HDD:500;WIFI:true;FAN:1;VGA:1;LCD:1920X1080";
    string str;
    char buf[128] = {0};
    int cnt = 0;

    for (unsigned int i = 0; i < strlen(info); i++) {
        CFG_INFO_T entry;

        if (info[i] != ';') {
            buf[cnt++]  = info[i];
        } else {
            buf[cnt] = '\0';
            str = string(buf);

            int idx = str.find_first_of(':');
            entry.key = str.substr(0, idx);
            entry.val = str.substr(idx+1, str.length()-idx-1);
            cfgInfos.push_back(entry);

            cnt = 0;
        }
    }

    if (cnt != 0) {
        CFG_INFO_T entry;

        buf[cnt] = '\0';
        str = string(buf);

        int idx = str.find_first_of(':');
        entry.key = str.substr(0, idx);
        entry.val = str.substr(idx+1, str.length()-idx-1);
        cfgInfos.push_back(entry);
        mlog("last add entry: key@[%s]\t val@[%s]", entry.key.c_str(), entry.val.c_str());
    }

}

/***********getter*************/
bool HWConfig::getBoolValue(const char *key)
{
    const char *str = getStringValue(key);

    if (str == NULL) {
        cout<<"notfound:"<<key<<endl;
        return false;
    }

    if (strcmp(str,"true") == 0) {
        return true;
    } else {
        return false;
    }
}

int HWConfig::getIntValue(const char *key)
{
    const char *str = getStringValue(key);

    if (!str) {
        return -1;
    } else {
        return atoi(str);
    }
}

float HWConfig::getFloatValue(const char* key)
{
    const char *str = getStringValue(key);

    if(str == NULL) {
        cout<<"notfound:"<<key<<endl;
        return -1.0;
    }

    return atof(str);
}

const char* HWConfig::getStringValue(const char *key)
{
    mlog("finding key[%s]", key);
    for(unsigned int i = 0; i < cfgInfos.size(); i++) {
        if(strcmp(cfgInfos[i].key.c_str(), key) == 0) {
            mlog("finded key[%s]", cfgInfos[i].key.c_str());
            return cfgInfos[i].val.c_str();
        }
    }

    cout<<"DEBUG: ["<<key<<"] not found"<<endl;
    return NULL;
}

/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
    log("--------print All loaded Entry------------");
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
        CFG_INFO_T entry = *it;
        log("  key:%s\t val:%s", entry.key.c_str(), entry.val.c_str());
    }
    log("--------print All loaded Entry------------");
}

