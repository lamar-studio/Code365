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

int control(const char *arg, char *retbuf)
{
    CHECK_INIT_STR(retbuf);
    rjlog_info("function enter.");

    g_callback("control", 1, "control_callback");
    strcpy(retbuf, "Hello, ");
    strcat(retbuf, arg);

    rjlog_info("function exit.");

    return 0;
}

int getBrightness()
{
    CHECK_FUNCTION_IN();
    rjlog_info("LINZR TEST.");
    CHECK_FUNCTION_OUT();
    return 0;
}

int setBrightness(int brightness)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getVoiceVolume()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setVoiceVolume(int volume)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getHdmiVoiceStatus()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setHdmiVoiceStatus(int status)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getCurrentResolution(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setDeviceResolution(int xres, int yres, int refresh)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getSupportResolution(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int shutDown()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int reboot()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setPowerState(int powerState)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getPowerState()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getSleeptime()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setSleeptime(int time)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getLedStatus(int color)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setLedStatus(int color, int state)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getHostname(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setHostname(const char *hostname)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setSystemTime(const char *time)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int setLanguage(const char *language)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int startConsole()
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getUsbPathForOffine(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}



#ifdef __cplusplus
}
#endif
