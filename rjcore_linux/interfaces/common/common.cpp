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

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

//typedef int(*callback)(const char *eventName, int eventType, const char *eventContent);
callback g_callback = NULL;
bool g_isinit = false;
static string g_platform;

#ifdef __cplusplus
extern "C" {
#endif

int Init(const char *comName, callback cb)
{
    int ret = 0;

    g_callback = cb;
   
    /* log init */ 
    ret = rjlog_init(comName);
    if (ret != 0) {
        return -1;
    }

    g_isinit = true;
    rjlog_info("rjcore_linux Version v%s init success.", STRINGIZE_VALUE_OF(VERSION));
      
    return 0;
}
 
#ifdef __cplusplus
}
#endif
