#define MODULE_TAG    "application"
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

int application()
{
    CHECK_INIT_INT();
    rjlog_info("functions enter.");

    g_callback("application", 1, "app_callback");

    rjlog_info("functions exit.");
    return 0;
}


int isProcessRunning(const char *processname)
{
    return 0;
}

int installApk_block(const char *apkPath)
{
    return 0;
}

int uninstallApk_block(const char *packageName)
{
    return 0;
}

int installDeb_block(const char *debPath)
{
    return 0;
}

int uninstallDeb_block(const char *debPath)
{
    return 0;
}

int mergeDeltaPacket()
{
    return 0;
}



#ifdef __cplusplus
}
#endif
