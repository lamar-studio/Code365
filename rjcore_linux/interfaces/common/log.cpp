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

#ifdef __cplusplus
extern "C" {
#endif

void logEmerg(const char *logInfo)
{
    rjlog_emerg("%s", logInfo);
}

void logError(const char *logInfo)
{
    rjlog_error("%s", logInfo);
}

void logWarn(const char *logInfo)
{
    rjlog_warn("%s", logInfo);
}

void logInfo(const char *logInfo)
{
    rjlog_info("%s", logInfo);
}

void logDebug(const char *logInfo)
{
    rjlog_debug("%s", logInfo);
}
 
#ifdef __cplusplus
}
#endif
