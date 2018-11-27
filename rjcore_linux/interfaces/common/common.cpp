#define MODULE_TAG     "init"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "rj_commom.h"

using namespace std;

//typedef int(*callback)(const char *eventName, int eventType, const char *eventContent);
static callback g_callback = NULL;
static bool g_isinit = false;

#ifdef __cplusplus
extern "C" {
#endif
bool isInit()
{
    return g_isinit;
}

int Init(const char *comName, callback cb)
{
    int ret = 0;

    g_callback = cb;

    /* log init */
    ret = rjlog_init(comName);
    if (ret != 0) {
        return ERROR_10001;
    }

    /* product init */
    ret = productInit();
    if (ret != 0) {
        return ERROR_10001;
    }

    ret = controlInit();
    if (ret != 0) {
        return ERROR_10001;
    }

    /* network init */
    network_init(cb);

    g_isinit = true;
    rjlog_info("rjcore_linux Version v%s init success.", STRINGIZE_VALUE_OF(VERSION));

    return SUCCESS_0;
}

#ifdef __cplusplus
}
#endif
