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
    char cmd[SIZE_128] = {0};
    char res[SIZE_128] = {0};

    snprintf(cmd, sizeof(cmd), "pidof %s", processname);
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] != '\0') {
        return 1;
    }

    return 0;
}

int startProcess(const char *path, const char *processname)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(path);
    CHECK_STR_PARA(processname);
    int len = 0;
    char res[SIZE_128] = {0};
    char cmd[SIZE_128] = {0};
    char pid_name[SIZE_128] = {0};
    char cmd_merge[SIZE_128] = {0};
    char bin_file[SIZE_128] = {0};

    len = strlen(path);
    if (path[len - 1] != '/') {
        rjlog_error("the path:%s is invalid", path);
        return ERROR_11003;
    }

    snprintf(cmd_merge, sizeof(cmd_merge), "%s%s", path, processname);
    sscanf(cmd_merge, "%s ", bin_file);
    if (access(bin_file, F_OK) != 0) {
        rjlog_error("Not found file:%s", bin_file);
        return ERROR_11000;
    }

    sscanf(processname, "%s ", pid_name);
    if (pid_name[0] != '\0') {
        snprintf(cmd, sizeof(cmd), "pidof %s", pid_name);
    } else {
        snprintf(cmd, sizeof(cmd), "pidof %s", processname);
    }
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] != '\0') {
        rjlog_warn("%s is running", cmd);
        return SUCCESS_0;
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s >/dev/null 2>&1 &", cmd_merge);
    rjlog_info("cmd:%s", cmd_merge);
    if (rj_system(cmd) < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    memset(cmd, 0, sizeof(cmd));
    memset(pid_name, 0, sizeof(pid_name));
    sscanf(processname, "%s ", pid_name);
    if (pid_name[0] != '\0') {
        snprintf(cmd, sizeof(cmd), "pidof %s", pid_name);
    } else {
        snprintf(cmd, sizeof(cmd), "pidof %s", processname);
    }
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_error("%s not found", cmd);
        return ERROR_11002;
    }

    return SUCCESS_0;
}

int installDeb_block(const char *debPath, const char *debName)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(debPath);
    CHECK_STR_PARA(debName);
    int ret = 0;
    int len = 0;
    char res[SIZE_128] = {0};
    char cmd[SIZE_128] = {0};
    char file[SIZE_1K] = {0};

    len = strlen(debPath);
    if (debPath[len - 1] != '/') {
        rjlog_error("the path:%s is invalid", debPath);
        return ERROR_11003;
    }

    snprintf(file, sizeof(file), "%s%s.deb", debPath, debName);
    if (access(file, F_OK) != 0) {
        rjlog_error("Not found file:%s", file);
        return ERROR_11000;
    }

    snprintf(cmd, sizeof(cmd), "dpkg -i %s >/dev/null 2>&1", file);
    ret = rj_exec_result(cmd, res, sizeof(res));
    if (ret < 0) {
        rjlog_error("cmd:%s error", cmd);
        return ERROR_10000;
    }

    memset(cmd, 0, sizeof(cmd));
    memset(res, 0, sizeof(res));
    snprintf(cmd, sizeof(cmd), "dpkg -l %s 2>/dev/null", debName);
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] != '\0') {
        return SUCCESS_0;
    } else {
        rjlog_error("dpkg install fail");
        return ERROR_11002;
    }

}

int uninstallDeb_block(const char *debName)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(debName);
    char res[SIZE_1K] = {0};
    char cmd[SIZE_128] = {0};

    snprintf(cmd, sizeof(cmd), "dpkg -r %s 2>&1", debName);
    rj_exec_result(cmd, res, sizeof(res));
    if (strstr(res, "isn't installed") != NULL) {
        rjlog_warn("this deb:%s isn't installed", debName);
        return ERROR_11002;
    }

    memset(cmd, 0, sizeof(cmd));
    memset(res, 0, sizeof(res));
    snprintf(cmd, sizeof(cmd), "dpkg -l %s 2>/dev/null", debName);
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        return SUCCESS_0;
    } else {
        rjlog_error("dpkg remove fail");
        return ERROR_11002;
    }

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
    char res[SIZE_1K] = {0};
    char cmd[SIZE_128] = {0};

    snprintf(cmd, sizeof(cmd), "bspatch %s %s %s 2>&1", old, newpack, delta);
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        return SUCCESS_0;
    } else {
        rjlog_error("cmd:%s has exception,please check", cmd);
        return ERROR_11002;
    }

}

#ifdef __cplusplus
}
#endif
