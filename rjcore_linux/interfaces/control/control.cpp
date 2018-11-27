#define MODULE_TAG    "control"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>

#include "rj_commom.h"
#include "dev_api.h"
#include "rw_bios_nv_user.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define BRIGHTNESS_PATH        "/sys/class/backlight/intel_backlight/"

static int bn_min, bn_max;
static rcdev::AudioManager am;
static rcdev::VideoManager vm;
static Mutex g_cfg_lock;

static int rj_set_int_conf(char *key, int val)
{
    int ret = 0;
    dictionary *ini = NULL;
    FILE *fp = NULL;

    AutoMutex Autolock(g_cfg_lock);
    ini = iniparser_load(RJ_USR_CONFIG_PATH "system.ini");
    if (ini == NULL) {
        rjlog_error("iniparser_load fail");
        ret = -1;
        goto out;
    }

    fp = fopen(RJ_USR_CONFIG_PATH "system.ini", "w+");
    if (fp == NULL) {
        rjlog_error("open system.ini fail");
        ret =  -1;
        goto out;
    }
    ret = iniparser_set_int(ini, key, val);
    if (ret < 0) {
        rjlog_error("iniparser_set_int fail");
        goto out;
    }

    iniparser_dump_ini(ini, fp);              //save

out:
    if (fp) {
        fflush(fp);
        fsync(fileno(fp));
        fclose(fp);
        fp = NULL;
    }

    if (ini) {
        iniparser_freedict(ini);
        ini = NULL;
    }

    return ret;
}

static int rj_get_int_conf(const char *key, int notfound)
{
    int val;
    dictionary *ini = NULL;

    AutoMutex Autolock(g_cfg_lock);
    CHECK_FUNCTION_IN();
    ini = iniparser_load(RJ_USR_CONFIG_PATH "system.ini");
    if (ini == NULL) {
        rjlog_error("iniparser_load fail");
        return notfound;
    }

    val = iniparser_getint(ini, key, notfound);

    if (ini) {
        iniparser_freedict(ini);
        ini = NULL;
    }

    return val;
}

static int is_support_brightness()
{
    if (access(BRIGHTNESS_PATH "brightness", F_OK) != 0) {
        rjlog_warn("this terminal nonsupport brightness.");
        return 0;
    } else {
        rjlog_info("this terminal support brightness.");
        return 1;
    }
}

static bool is_hdmi_connected()
{
    char res[SIZE_64] = {0};

    rj_exec_result("cat `find /sys/class/drm/ | grep HDMI | sed 's/$/\\/status/g'` | grep \"^connected\"",
                   res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_info("HDMI device disconnected.");
        return false;
    } else {
        rjlog_info("HDMI device connected.");
        return true;
    }
}

static int brightness_map()
{
    int len = 0;
    char res[SIZE_64] = {0};

    rj_exec_result("cat " BRIGHTNESS_PATH "max_brightness", res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_error("the max_brightness is empty");
        return ERROR_11002;
    }

    len = strlen(res);
    if (res[len - 1] == '\n') {
        res[len - 1] = '\0';
    }
    bn_max = atoi(res);
    bn_min = bn_max / 20;

    return 0;
}

int controlInit()
{
    CHECK_FUNCTION_IN();
    int ret = 0;

    if (is_support_brightness()) {
        ret = brightness_map();
    }

    return ret;
}

int getBrightness()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    int brightness = 0;

    if (is_support_brightness()) {
        brightness = rj_get_int_conf("control:brightness", 81);   // 81 - the default val.
    }

    return brightness;
}

int setBrightness(int brightness)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char cmd[SIZE_64] = {0};
    int actual = 0;

    if (brightness < 0 || brightness > 100) {
        rjlog_error("param error: 0 <= brightness <= 100");
        return ERROR_10000;
    }

    if (is_support_brightness()) {
        actual = math_util::percentage_to_value(brightness, bn_min, bn_max);
        rjlog_debug("actual:%d ui:%d", actual, brightness);

        snprintf(cmd, sizeof(cmd), "echo %d > %s",  actual, BRIGHTNESS_PATH "brightness");
        if (rj_system(cmd) < 0) {
            rjlog_error("cmd:%s error", cmd);
            return ERROR_10000;
        }

        // save the brightness to ini.
        if (rj_set_int_conf((char *)"control:brightness", brightness) < 0) {
            rjlog_error("rj_set_conf error");
            return ERROR_10000;
        }
    }

    return SUCCESS_0;
}

int isSupportBrightness()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    return is_support_brightness();
}

int startPaService(const char *type)
{
    CHECK_FUNCTION_IN();

    int ret = am.startPaService(type);
    if (ret < 0) {
        rjlog_error("connect pulse fail");
        return ERROR_10000;
    }

    return ret;
}

int getVoiceVolume()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

#if 0
    int vol = am.getVolume();
    if (vol < 0) {
        rjlog_error("get volume fail");
        return ERROR_10000;
    }
#endif
    return SUCCESS_0;
}

int setVoiceVolume(int volume)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
#if 0
    if (volume < 0 || volume > 100) {
        rjlog_error("param error: 0 <= volume <= 100");
        return ERROR_10000;
    }

    int ret = am.setVolume(volume);
    if (ret < 0) {
        rjlog_error("set volume fail");
        return ERROR_10000;
    }
#endif
    return SUCCESS_0;
}

int getHdmiVoiceStatus()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    int ret = rj_get_int_conf("control:hdmi", 0);
    rjlog_debug("control:hdmi val:%d ", ret);

    return ret;
}

int setHdmiVoiceStatus(int status)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (!is_hdmi_connected()) {
        return ERROR_11001;
    }

    bool bHDMI = status;

    int ret = am.changeHDMI(bHDMI ? "hdmi" : "analog");
    if (ret < 0) {
        rjlog_error("changeHDMI fail");
        return ERROR_10000;
    }

    if (rj_set_int_conf((char *)"control:hdmi", bHDMI) < 0) {
        rjlog_error("rj_set_conf:hdmi error");
        return ERROR_10000;
    }

    return SUCCESS_0;
}

int getCurrentResolution(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    string cur = vm.getCurrentResolution();

    if (cur.empty()) {
        snprintf(retbuf, size, "%s", rj_strerror(ERROR_10000).c_str());
        return ERROR_10000;
    }
    snprintf(retbuf, size, "%s", cur.c_str());

    return SUCCESS_0;
}

int setDeviceResolution(const char *res, int refresh)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(res);
    int ret = vm.setResolution(res);
    if (ret < 0) {
        rjlog_error("setResolution fail");
        return ERROR_10000;
    }

    return ret;
}

int getSupportResolution(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    string support = vm.getSupportResolution();

    if (support.empty()) {
        snprintf(retbuf, size, "%s", rj_strerror(ERROR_10000).c_str());
        return ERROR_10000;
    }
    snprintf(retbuf, size, "%s", support.c_str());

    return SUCCESS_0;
}

int getOptimumResolution(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    string optimum = vm.getOptimumResolution();

    if (optimum.empty()) {
        snprintf(retbuf, size, "%s", rj_strerror(ERROR_10000).c_str());
        return ERROR_10000;
    }
    snprintf(retbuf, size, "%s", optimum.c_str());

    return SUCCESS_0;
}

int shutDown()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (rj_system("shutdown -h now") < 0) {
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int reboot()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (rj_system("reboot") < 0) {
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int setPowerState(int powerState)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char type[SIZE_64] = {0};
    bool startAuto = powerState;

    getTermialType(type, sizeof(type));

    // vdi function
    if (strncmp(type, RJ_VDI_TYPE, sizeof(RJ_VDI_TYPE)) == 0) {
        if (startAuto) {
            vdi_bios_nv_write_user(AC_POWER_VAR_NAME, 1);
        } else {
            vdi_bios_nv_write_user(AC_POWER_VAR_NAME, 0);
        }
    // idv function
    } else if (strncmp(type, RJ_IDV_TYPE, sizeof(RJ_IDV_TYPE)) == 0) {
        if (startAuto) {
            idv_bios_nv_write_user(AC_POWER_VAR_NAME, 1);
        } else {
            idv_bios_nv_write_user(AC_POWER_VAR_NAME, 0);
        }
    } else {
        rjlog_info("nonsupport this  termial type:%s", type);
        return ERROR_11001;
    }

    rjlog_info("bios_nv_write_user:%d", startAuto);

    return SUCCESS_0;
}

int getPowerState()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char type[SIZE_64] = {0};
    unsigned int nv_value = 0;

    getTermialType(type, sizeof(type));

    // vdi function
    if (strncmp(type, RJ_VDI_TYPE, sizeof(RJ_VDI_TYPE)) == 0) {
        vdi_bios_nv_read_user(AC_POWER_VAR_NAME, &nv_value);
    // idv function
    } else if (strncmp(type, RJ_IDV_TYPE, sizeof(RJ_IDV_TYPE)) == 0) {
        idv_bios_nv_read_user(AC_POWER_VAR_NAME, &nv_value);
    } else {
        rjlog_info("nonsupport this termial type:%s", type);
        return ERROR_11001;
    }

    rjlog_info("bios_nv_read_user:%d", nv_value);

    if ((bool)nv_value) {
        return 1;
    } else {
        return 0;
    }

}

int setSleeptime(int second)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char cmd[SIZE_64] = {0};

    if (second < 0) {
        rjlog_error("the para:%d is invalid", second);
        return ERROR_11003;
    }

    snprintf(cmd, sizeof(cmd), "xset -s %d", second);
    if (rj_system(cmd) < 0) {
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int getHostname(char *hostname, size_t size)
{
    CHECK_INIT_STR(hostname, size);
    CHECK_FUNCTION_IN();
    int len = 0;
    char cmd[SIZE_128] = {0};
    char res[SIZE_128] = {0};

    if (access(RJ_USR_CONFIG_PATH "hostname.ini", F_OK) != 0) {
        rjlog_info("Not found ini, Creat it.");
        snprintf(cmd, sizeof(cmd), "echo `hostname` > %s", RJ_USR_CONFIG_PATH "hostname.ini");
        if (rj_exec_result(cmd, res, sizeof(res)) < 0) {
            rjlog_error("cmd:%s error", cmd);
            snprintf(hostname, size, "%s", rj_strerror(ERROR_11002).c_str());
            return ERROR_11002;
        }
    }

    memset(res, 0, sizeof(res));
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "cat %s", RJ_USR_CONFIG_PATH "hostname.ini");
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_warn("the hostname.ini is empty");
        snprintf(hostname, size, "%s", rj_strerror(ERROR_11002).c_str());
        return ERROR_11002;
    }

    len = strlen(res);
    if (res[len - 1] == '\n') {
        res[len - 1] = '\0';
    }

    snprintf(hostname, size, "%s", res);

    return SUCCESS_0;
}

int setHostname(const char *hostname)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char cmd[SIZE_128] = {0};

    if (NULL == hostname || strlen(hostname) > 0x80) {
        rjlog_error("the para is invalid");
        return ERROR_11003;
    }

    snprintf(cmd, sizeof(cmd), "echo %s > %s", hostname, RJ_USR_CONFIG_PATH "hostname.ini");
    if (rj_system(cmd) < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int syncServerTime_block(const char *serverip)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    if (check_ipaddr(serverip) != 0) {
        rjlog_error("the ip:%s is invalid", serverip);
        return ERROR_11003;
    }
    char cmd[SIZE_64] = {0};

    //sync the time from server
    snprintf(cmd, sizeof(cmd), "ntpdate %s", serverip);
    if (rj_system(cmd) < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_11002;
    }

    //sync the time to hardware
    if (rj_system("hwclock --systohc") < 0) {
        rjlog_error("cmd:hwclock --systohc error", cmd);
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int setLanguage(const char *language)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(language);
    if (access(RJ_SCRIPT_PATH "setLanguage", F_OK) != 0) {
        rjlog_error("setLanguage script Not found please Check.");
        return ERROR_11000;
    }
    char cmd[SIZE_128] = {0};

    if (strncmp(language, RJ_LANG_CHINESE, sizeof(RJ_LANG_CHINESE)) == 0) {
        snprintf(cmd, sizeof(cmd), "bash " RJ_SCRIPT_PATH "setLanguage " RJ_LANG_CHINESE);
        if (rj_system(cmd) < 0) {
            rjlog_error("cmd:%s error", cmd);
            return ERROR_11002;
        }
    } else if (strncmp(language, RJ_LANG_ENGLISH, sizeof(RJ_LANG_ENGLISH)) == 0) {
        snprintf(cmd, sizeof(cmd), "bash " RJ_SCRIPT_PATH "setLanguage " RJ_LANG_ENGLISH);
        if (rj_system(cmd) < 0) {
            rjlog_error("cmd:%s error", cmd);
            return ERROR_11002;
        }
    } else {
    	rjlog_error("nonsupport this language:%s", language);
        return ERROR_11001;
    }

    return SUCCESS_0;
}

int startConsole()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (rj_system("xterm -e login -p") < 0) {
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int getUsbPathForOffine(char *retbuf, size_t size)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return SUCCESS_0;
}

int mountUsb(const char *dev, char *retbuf, size_t size)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    return SUCCESS_0;
}

int getUsbList(char *retbuf, size_t size)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    return SUCCESS_0;
}

int unmountUsb(const char *path)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    return SUCCESS_0;
}

#ifdef __cplusplus
}
#endif
