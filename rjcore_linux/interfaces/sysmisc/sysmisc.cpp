#define MODULE_TAG    "sysmisc"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "rj_commom.h"
#include "json.h"
#include "rc_json.h"

using namespace std;
using namespace RcJson;

#ifdef __cplusplus
extern "C" {
#endif

int getPlatHWInfo(char *info, int len)
{
    CHECK_FUNCTION_IN();
    if (info == NULL){
        rjlog_error("info is NULL.");
        return ERROR_10000;
    }
    if (rj_system_r_args(info, len, "bash /usr/local/bin/system/getHWCfg") == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to getHWCfg");
    return ERROR_10000;
}

int getDiskStatus_block()
{
    int ret;
    CHECK_FUNCTION_IN();
    ret = rj_system_args("bash /usr/local/bin/system/diskstatus_mgr --status");
    if (ret == 0){
        return SUCCESS_0;
    }else if (ret > 0){
        return ERROR_10000 - ret;
    }
    return ERROR_10000;
}

int getPersonalDiskList_block(char *list, int len)
{
    CHECK_FUNCTION_IN();
    if (list == NULL){
        rjlog_error("list is NULL.");
        return ERROR_10000;
    }
    if (rj_system_r_args(list, len, "bash /usr/local/bin/system/getDataDiskInfoList") == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to getDataDiskInfoList");
    return ERROR_10000;
}

int formatPersonalDisk_block(const char *dev)
{
    CHECK_FUNCTION_IN();
    if (dev == NULL){
        rjlog_error("dev is NULL.");
        return ERROR_10000;
    }
    if (rj_system_args("%s %s", "bash /usr/local/bin/system/formatDataDiskDev", dev) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to formatDataDiskDev");
    return ERROR_10000;
}

int mountPersonalDisk_block(const char *dev,const char *dir, const char*option)
{
    CHECK_FUNCTION_IN();
    if (dev == NULL){
        rjlog_error("dev is NULL.");
        return ERROR_10000;
    }
    if (dir == NULL){
        rjlog_error("dir is NULL.");
        return ERROR_10000;
    }
    if (rj_system_args("%s %s %s", "bash /usr/local/bin/system/mountDataDisk", dev, dir, (option != NULL ? option : "")) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to mountDataDisk");
    return ERROR_10000;
}

int umountPersonalDisk_block(const char *dir)
{
    CHECK_FUNCTION_IN();
    if (dir == NULL){
        rjlog_error("dir is NULL.");
        return ERROR_10000;
    }
    if (rj_system_args("%s %s", "bash /usr/local/bin/system/umountDataDisk", dir) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to umountDataDisk");
    return ERROR_10000;

}

int checkISOVersion(const char *verstr)
{
    CHECK_FUNCTION_IN();
    if (verstr == NULL){
        rjlog_error("verstr is NULL.");
        return ERROR_10000;
    }
    if (rj_system_args("%s %s", "bash /usr/local/bin/system/checkISOVersion", verstr) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to checkISOVersion");
    return ERROR_10000;
}

#define FASTUPGRADE_PARAM_JSON(out, jsonmsg, key)      {out = rc_json_get_string(jsonmsg, key);  \
                                                            if (out.empty()){ \
                                                                rjlog_error(key" is NULL."); \
                                                                return ERROR_10000; \
                                                            }}
int fastUpgrade(const char*jsonmsg)
{
    string ip, mask, gateway, ser_ip, mode, img, postrun, isover;    
    CHECK_FUNCTION_IN();
    if (jsonmsg == NULL){
        rjlog_error("jsonmsg is NULL.");
        return ERROR_10000;
    }
    FASTUPGRADE_PARAM_JSON(ip, jsonmsg, "ip");
    FASTUPGRADE_PARAM_JSON(mask, jsonmsg, "mask");
    FASTUPGRADE_PARAM_JSON(gateway, jsonmsg, "gateway");
    FASTUPGRADE_PARAM_JSON(ser_ip, jsonmsg, "ser_ip");
    FASTUPGRADE_PARAM_JSON(mode, jsonmsg, "mode");
    FASTUPGRADE_PARAM_JSON(img, jsonmsg, "img_name");
    FASTUPGRADE_PARAM_JSON(postrun, jsonmsg, "postrun");
    FASTUPGRADE_PARAM_JSON(isover, jsonmsg, "isover");
    if (rj_system_args("bash /usr/local/bin/system/fast_upgrade %s %s %s %s %s %s %s %s", ip.c_str(), mask.c_str(), gateway.c_str(),
                         ser_ip.c_str(), mode.c_str(), img.c_str(), postrun.c_str(), isover.c_str()) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to ipxe_upgrade");
    return ERROR_10000;
}
#undef FASTUPGRADE_PARAM_JSON

int ipxeUpgrade(const char *verstr)
{
    CHECK_FUNCTION_IN();
    if (verstr == NULL){
        rjlog_error("verstr is NULL.");
        return ERROR_10000;
    }
    if (rj_system_args("%s %s", "bash /usr/local/bin/system/ipxe_upgrade", verstr) == 0){
        return SUCCESS_0;
    }
    rjlog_error("Failed to ipxe_upgrade");
    return ERROR_10000;
}

#ifdef __cplusplus
}
#endif

