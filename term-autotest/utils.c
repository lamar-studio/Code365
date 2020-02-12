
#include "utils.h"


/**
 * Function: rj_popen
 * Description: same as popen, and auto write log
 */
static FILE *rj_popen(const char* command, const char* mode) {
    FILE *fp = NULL;

    if (command == NULL) {
        printf("command is null");
        return NULL;
    }
    if (mode == NULL) {
        printf("mode is not set");
        return NULL;
    }

    fp = popen(command, mode);
    if (fp == NULL) {
        printf("popen failed,errno:%d: %s", errno, strerror(errno));
        return NULL;
    }

    return fp;
}

/**
 * Function: rj_pclose
 * Description: same as pclose, and auto write log
 * Return:     <0   failed to run cmd
 *             >=0  cmd return value
 */
static int rj_pclose(FILE *fp) {
    int ret;
    if (fp == NULL){
        //rjlog_error("invalid ptr");
        return -1;
    }

    ret = pclose(fp);
    if (ret < 0) {
        if (errno == ECHILD) {  /* No child processes SIGCHLD */
            printf("ignore this errno:%d: %s", errno, strerror(errno));

            return 0;
        }

        printf("errno:%d: %s", errno, strerror(errno));
        return ret;
    }

    if (WIFSIGNALED(ret)) {
        printf("popen command abnormal,signal number:%d %s", WTERMSIG(ret), strsignal(WTERMSIG(ret)));
        return -1;
    } else if (WIFSTOPPED(ret)) {
        printf("popen command stop,signal number:%d %s", WSTOPSIG(ret), strsignal(WSTOPSIG(ret)));
        return -1;
    } else if (WIFEXITED(ret)) {
        ret = WEXITSTATUS(ret);
        if (ret) {
            printf("popen command exit status:%d", ret);
        }
    }

    return ret;
}

int rj_system(const char *command) {
    FILE *fd;

    fd = rj_popen(command, "w");
    if (fd != NULL) {
        return rj_pclose(fd);
    }

    printf("rj_popen failed");
    return -1;
}

/**
 * Function: rj_exec_result
 * Description: run system cmd with read mode
 * Return:     <0   failed to run cmd
 *             >=0  cmd return value
 * result: the result of cmd, you should memset 0 to result before use it!!!
 */
int exec_result(const char *cmd, char *result, size_t size) {
    if (cmd == NULL || *cmd == '\0' || result == NULL) {
        return -1;
    }

    char *pBuf = result;
    int len = 0;
    int ret = 0;
    FILE *ptr = NULL;

    if ((ptr = rj_popen(cmd, "r")) != NULL) {
        while (fgets(pBuf, size, ptr) != NULL) {
            if ((len = strlen(pBuf)) == 0) {
                break;
            }

            size -= len;
            pBuf += len;
        }

        ret = rj_pclose(ptr);
        ptr = NULL;
    } else {
        return -1;
    }

    return ret;
}










