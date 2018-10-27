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
    return 0;
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
