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

int isProcessRunning(const char *processname)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(processname);
    char cmd[128] = {0};
    char res[128] = {0};

    snprintf(cmd, sizeof(cmd), "pidof %s", processname);
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] != '\0') {
        return 1;
    }

    return 0;
}

int startProcess(const char *processname)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(processname);
    char cmd[128] = {0};

    snprintf(cmd, sizeof(cmd), "%s &", processname);
    rjlog_info("cmd:%s", processname);
    if (rj_system(cmd) < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    return 0;
}

int installDeb_block(const char *debPath)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(debPath);
    int ret = 0;
    char cmd[128] = {0};

    snprintf(cmd, sizeof(cmd), "dpkg -i %s", debPath);
    ret = rj_system(cmd);
    if (ret < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    return 0;
}

int uninstallDeb_block(const char *debPath)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(debPath);
    int ret = 0;
    char cmd[128] = {0};

    snprintf(cmd, sizeof(cmd), "dpkg -r %s", debPath);
    ret = rj_system(cmd);
    if (ret < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    return 0;
}

int mergeDeltaPacket_block(const char *old, const char *delta, const char *newpack)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(old);
    CHECK_STR_PARA(delta);
    if (access(old, F_OK) != 0 || access(delta, F_OK) != 0) {
        rjlog_error("Not found file");
        return ERROR_11000;
    }
    int ret = 0;
    char cmd[128] = {0};

    snprintf(cmd, sizeof(cmd), "bspatch %s %s %s", old, newpack, delta);
    ret = rj_system(cmd);
    if (ret < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
