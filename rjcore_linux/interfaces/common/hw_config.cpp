/*
 * Create by LaMar at 2018/09/05
 */

#include "rj_commom.h"
#include "hw_config.h"

using namespace std;

static vector<CFG_INFO_T> cfgInfos;

HWConfig::HWConfig() {
    loadHwConfigInfo();
}

HWConfig::~HWConfig() {
}

void HWConfig::loadHwConfigInfo()
{
    char info[1024] = {0};
    string str;
    char buf[128] = {0};
    char cmd[128] = {0};
    int cnt = 0;

    if (access(RJ_SYS_CONFIG_PATH "hwcfg.ini", F_OK) != 0) {
        rjlog_error("not found the config file");
        return;
    }

    snprintf(cmd, sizeof(cmd), "bash " RJ_SCRIPT_PATH "getHWCfg");
    rj_exec_result(cmd, info, sizeof(info));
    if (info[0] == '\0') {
        rjlog_error("cannot get the hardware config");
        return;
    }

    sscanf(info, "{%[^}]", info);
    rjlog_info("the hardware info:%s", info);

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
        rjlog_info("last add entry: key@[%s]\t val@[%s]", entry.key.c_str(), entry.val.c_str());
    }

}

/***********getter*************/
bool HWConfig::getBoolValue(const char *key)
{
    const char *str = getStringValue(key);

    if (str[0] == '\0') {
        rjlog_error("notfound key[%s]", key);
        return false;
    }

    if (strcmp(str, "true") == 0) {
        return true;
    } else {
        return false;
    }
}

int HWConfig::getIntValue(const char *key)
{
    const char *str = getStringValue(key);

    if (str[0] == '\0') {
        rjlog_error("notfound key[%s]", key);
        return -1;
    }

    return atoi(str);
}

float HWConfig::getFloatValue(const char* key)
{
    const char *str = getStringValue(key);

    if(str[0] == '\0') {
        rjlog_error("notfound key[%s]", key);
        return -1.0;
    }

    return atof(str);
}

const char* HWConfig::getStringValue(const char *key)
{
    for(unsigned int i = 0; i < cfgInfos.size(); i++) {
        if(strcmp(cfgInfos[i].key.c_str(), key) == 0) {
            rjlog_info("finded key[%s]", cfgInfos[i].key.c_str());
            return cfgInfos[i].val.c_str();
        }
    }
    return "";
}

/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
    rjlog_debug("--------print All loaded Entry------------");
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
        CFG_INFO_T entry = *it;
        rjlog_debug("  key:%s\t val:%s", entry.key.c_str(), entry.val.c_str());
    }
    rjlog_debug("--------print All loaded Entry------------");
}

