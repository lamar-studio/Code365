#define MODULE_TAG "rj_log"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h> 
#include <pthread.h>
#include "rj_log.h"

#define RJ_LOG_MAX_LEN     1024
#define MODULE_NAME        "rjcore_linux"
#define FILE_NAME_MAX      1024
#define LOG_DIR            "/var/log/rjcore_linux"


typedef void (*rj_log_callback)(const char*, const char*, va_list);
static const char *log_level[RJ_MAX] = { (char*)"EMERG", (char*)"ALERT", (char*)"CRIT ", \
                                          (char*)" ERR ", (char*)"WARN ", (char*)"NOTICE", \
                                          (char*)"INFO ", (char*)"DEBUG"
                                        };

U32 rj_debug = 0;//0x10000000;
static U32 rj_log_flag = 0;

static const char *msg_log_warning = "log message is long\n";
static const char *msg_log_nothing = "\n";
static FILE *rj_log_fp = NULL;
static pthread_mutex_t mutex;
static uint64_t logsize_cur = 0;
static uint64_t logsize_max = 0;
char log_file_path[FILE_NAME_MAX] = {0};
char log_file_bak_path[FILE_NAME_MAX] = {0};
//static time_t timep;

#define LINE_SZ 1024
#define CMD_SZ 1024

void os_log(const char* tag, const char* msg, va_list list)
{
    int len = 0;
    char line[LINE_SZ] = {0};
    char cmd[CMD_SZ] = {0};
    int ret;
    FILE *fp = rj_log_fp ? rj_log_fp : stdout;

    if (rj_log_fp)
        pthread_mutex_lock(&mutex);

    snprintf(line, sizeof(line), "%s", msg);
    len = vfprintf(fp, line, list);
    fflush(fp);

    if (len > 0) {
        logsize_cur += len;
        if (logsize_cur >= logsize_max) {
            snprintf(cmd, CMD_SZ, "cp -f %s %s", log_file_path, log_file_bak_path);
            rj_exec(cmd);
            ret = truncate(log_file_path, 0);
            if (ret != 0) {
                goto exit;
            }
            rewind(fp);
            logsize_cur = 0;
        }
    }

exit:
    if (rj_log_fp)
        pthread_mutex_unlock(&mutex);
}


void __rj_log(rj_log_callback func, const char *tag, const char *fname, const char *function, int line, int level, const char *fmt, va_list args)
{
    char msg[RJ_LOG_MAX_LEN + 1];
    char *tmp = msg;
    const char *buf = fmt;

    size_t len_fmt  = strnlen(fmt, RJ_LOG_MAX_LEN);
    size_t file_len = (fname) ? (strnlen(fname, RJ_LOG_MAX_LEN)) : (0);
    size_t func_len = (function) ? (strnlen(function, RJ_LOG_MAX_LEN)) : (0);
    size_t line_len = (line >= 0) ? (sizeof(int)) : (0);
    size_t buf_left = RJ_LOG_MAX_LEN;
    size_t len_all  = len_fmt;
    size_t len = 0;

    if (NULL == tag)
        tag = MODULE_TAG;

    {
        struct timeval  tv;
        struct timezone tz;
        struct tm nowtime;
        gettimeofday(&tv, &tz);
        //time (&timep);
        localtime_r(&tv.tv_sec, &nowtime);
        char tmp_buf[128];
        strftime(tmp_buf, sizeof(tmp_buf), "%m-%d %H:%M:%S", &nowtime);
        len = snprintf(tmp, buf_left, "%s.%03ld", tmp_buf, tv.tv_usec >> 10);
        buf_left -= len;
        tmp += len;
        len_all += len;
    }

    if ((level >= 0) && (level < RJ_MAX)) {
        len = snprintf(tmp, buf_left, " <%s> ", log_level[level]);
        buf_left -= len;
        tmp += len;
        len_all += len;
    }

    if ((file_len) && (func_len) && (line_len)) {
        len = snprintf(tmp, buf_left, "[ %s:%s(%d) ] ", fname, function, line);
        buf_left -= len;
        tmp += len;
        len_all += len;
    }

    if (len_all == 0) {
        buf = msg_log_nothing;
    } else if (len_all >= RJ_LOG_MAX_LEN) {
        buf_left -= snprintf(tmp, buf_left, "%s", msg_log_warning);
        buf = msg;
    } else {
        snprintf(tmp, buf_left, "%s", fmt);
        if (fmt[len_fmt - 1] != '\n') {
            tmp[len_fmt]    = '\n';
            tmp[len_fmt + 1]  = '\0';
        }
        buf = msg;
    }

    func(tag, buf, args);
}

void _rj_log(const char *tag, const char *fname, const char *function, int line, int level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __rj_log(os_log, tag, fname, function, line, level, fmt, args);
    va_end(args);
}

int rjlog_open(const char* file)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    char cmd[CMD_SZ] = {0};

    rjlog_close();

    pthread_mutex_lock(&mutex);
    rj_log_fp = fopen(file, "a+");
    pthread_mutex_unlock(&mutex);

    if (rj_log_fp == NULL) {
        return -1;
    }
    
    snprintf(cmd, CMD_SZ, "chmod 0775 %s ", log_file_path);
    rj_exec(cmd);
    logsize_cur = rj_get_file_size(file);

    return 0;
}

void rjlog_close()
{
    if (rj_log_fp != NULL) {
        pthread_mutex_lock(&mutex);
        fclose(rj_log_fp);
        rj_log_fp = NULL;
        pthread_mutex_unlock(&mutex);
    }
}

void rjlog_set_flag(U32 flag)
{
    rj_log_flag = flag;
    return ;
}

U32 rjlog_get_flag()
{
    return rj_log_flag;
}

int rjlog_init(const char *comName)
{
    int ret = 0;

    if (comName == NULL || strcmp(comName, "") == 0) {
        return -1;
    }    

    ret = access(LOG_DIR, 0);
    if (ret != 0) {
        ret = mkdir(LOG_DIR, 0775);
        if (ret !=0) {
            return -1;
        }
    }

    snprintf(log_file_path, FILE_NAME_MAX, "%s/%s_%s.log", LOG_DIR, MODULE_NAME, comName);
    snprintf(log_file_bak_path, FILE_NAME_MAX, "%s/%s_%s_bak.log", LOG_DIR, MODULE_NAME, comName);
    ret = rjlog_open(log_file_path);
    if (ret != 0) {
        return -1;
    }

    if(strcmp(comName, "shine") == 0) {
        logsize_max = 5 * 1024 * 1024;        
    } else {
        logsize_max = 1 * 1024 * 1024;        
    }

    return 0;
}
