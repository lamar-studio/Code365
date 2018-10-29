#define MODULE_TAG    "product"
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

int product()
{
    CHECK_INIT_INT();
    rjlog_info("function enter.");

    g_callback("product", 1, "product_callback");

    rjlog_info("function exit.");
    return 0;
}

int getProductType(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getAbsLayerVersion(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getSystemVersion(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getHardwareVesion(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getProductId(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getSerialNumber(char *retbuf)
{
    CHECK_FUNCTION_IN();
    char result[128] = {0};
    char command[128] = {0};
    int ret = 0;

    sprintf(command, "dmidecode -t system | grep 'Serial Number'|cut -d : -f 2|sed s/[[:space:]]//g" );
    rj_exec_result(command, result, sizeof(result));
    if((strstr(result, "To be filled by O.E.M") != NULL) || (strstr(result, "empty") != NULL)) {
        ret = -1;
        goto end;
    }
    strncpy(retbuf, result, sizeof(result));

end:
    return ret;
}

int getMacAddress(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getBiosInfo(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getCpuInfo(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getMemoryInfo(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getBaseboardInfo(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getSystemBit(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int getTermialType(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int collectLog(char *retbuf)
{
    CHECK_FUNCTION_IN();
    return 0;
}

int rmLogFile(const char *path)
{
    CHECK_FUNCTION_IN();
    return 0;
}

#ifdef __cplusplus
}
#endif
