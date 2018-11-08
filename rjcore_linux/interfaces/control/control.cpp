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

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

int control(const char *arg, char *retbuf, int len)
{
    CHECK_INIT_STR(retbuf, len);
    rjlog_info("function enter.");

    g_callback("control", 1, "control_callback");
    strcpy(retbuf, "Hello, ");
    strcat(retbuf, arg);

    rjlog_info("function exit.");

    return 0;
}

int getBrightness()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    //TODO: implement

    return 0;
}

int setBrightness(int brightness)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    //TODO: implement
    return 0;
}

int getVoiceVolume()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setVoiceVolume(int volume)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getHdmiVoiceStatus()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setHdmiVoiceStatus(int status)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getCurrentResolution(char *retbuf)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setDeviceResolution(int xres, int yres, int refresh)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getSupportResolution(char *retbuf)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int shutDown()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (rj_system("shutdown -h now") < 0) {
        return ERROR_10000;
    }

    return 0;
}

int reboot()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    if (rj_system("reboot") < 0) {
        return ERROR_10000;
    }

    return 0;
}

int setPowerState(int powerState)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getPowerState()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getSleeptime()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setSleeptime(int time)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getLedStatus(int color)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setLedStatus(int color, int state)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getHostname(char *hostname, size_t size)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
	char cmd[128] = {0};
    char res[128] = {0};

	if (NULL == hostname || size < 2) {
		rjlog_error("the para is invalid");
		return ERROR_10000;
	}

    if (size > sizeof(res)) {
        size = sizeof(res);
    }

    if (access(RJ_USR_CONFIG_PATH "hostname.ini", F_OK) != 0) {
        rjlog_info("Not found ini, Creat it.");
        snprintf(cmd, sizeof(cmd), "echo `hostname` > %s", RJ_USR_CONFIG_PATH "hostname.ini");
        if (rj_system(cmd) < 0) {
            rjlog_error("cmd:%s error", cmd);
        }
    }

    snprintf(cmd, sizeof(cmd), "cat %s", RJ_USR_CONFIG_PATH "hostname.ini");
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_warn("the hostname.ini is empty");
        return ERROR_10000;
    }
    strncpy(hostname, res, size);

	return 0;
}

int setHostname(const char *hostname)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
	char cmd[0x200];

	if (NULL == hostname || strlen(hostname) > 0x80) {
		rjlog_error("the para is invalid");
		return ERROR_10000;
	}

    snprintf(cmd, sizeof(cmd), "echo %s > %s", hostname, RJ_USR_CONFIG_PATH "hostname.ini");
    if (rj_system(cmd) < 0) {
        rjlog_error("cmd:%s error", cmd);
    }

	if (0 != rj_system(cmd)){
		rjlog_error("cmd:%s error", cmd);
		return -1;
	}

	return 0;
}

int setSystemTime(const char *time)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int setLanguage(const char *language)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int startConsole()
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}

int getUsbPathForOffine(char *retbuf)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    return 0;
}



#ifdef __cplusplus
}
#endif
