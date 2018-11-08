#ifndef __RJ_LOG_H__
#define __RJ_LOG_H__

#include <stdio.h>
#include <stdlib.h>

#include "rj_type.h"
#include "rj_utils.h"

enum LOG_LEVEL {
    RJ_EMERG = 0,
    RJ_ALERT,
    RJ_CRIT,
    RJ_ERR,
    RJ_WARNING,
    RJ_NOTICE,
    RJ_INFO,
    RJ_DEBUG,
    RJ_MAX,
};


/*
 * runtime log system usage:
 * rj_err is for error status message, it will print for sure.
 * rj_log is for important message like open/close/reset/flush, it will print too.
 * rj_dbg is for all optional message. it can be controlled by debug and flag.
 */

#define rjlog_emerg(fmt, ...)     _rj_log(MODULE_TAG, __FILE__ , __FUNCTION__, __LINE__, RJ_EMERG, fmt, ## __VA_ARGS__)
#define rjlog_error(fmt, ...)     _rj_log(MODULE_TAG, __FILE__ , __FUNCTION__, __LINE__, RJ_ERR, fmt, ## __VA_ARGS__)
#define rjlog_warn(fmt, ...)      _rj_log(MODULE_TAG, __FILE__ , __FUNCTION__, __LINE__, RJ_WARNING, fmt, ## __VA_ARGS__)
#define rjlog_info(fmt, ...)      _rj_log(MODULE_TAG, __FILE__ , __FUNCTION__, __LINE__, RJ_INFO, fmt, ## __VA_ARGS__)
#define rjlog_debug(fmt, ...)     _rj_log(MODULE_TAG, __FILE__ , __FUNCTION__, __LINE__, RJ_DEBUG, fmt, ## __VA_ARGS__)


#define _rj_dbg(debug, flag, fmt, ...) \
             do { \
                if (debug & flag) \
                    rjlog_debug(fmt, ## __VA_ARGS__); \
             } while (0)

#define rj_dbg(flag, fmt, ...) _rj_dbg(rj_debug, flag, fmt, ## __VA_ARGS__)


#define RJ_ABORT                       (0x10000000)

#define rj_abort() do {                \
    if (rj_debug & RJ_ABORT) {        \
        abort();                        \
    }                                   \
} while (0)

#define RJ_STRINGS(x)      RJ_TO_STRING(x)
#define RJ_TO_STRING(x)    #x

#define rj_assert(cond) do {                                           \
    if (!(cond)) {                                                      \
        rjlog_emerg("Assertion %s failed at %s %s:%d\n",                       \
               RJ_STRINGS(cond), __FUNCTION__, __LINE__);              \
        rj_abort();                                                    \
    }                                                                   \
} while (0)


extern U32 rj_debug;

int rjlog_open(const char* file);
void rjlog_close();
void rjlog_set_flag(U32 flag);
U32 rjlog_get_flag();
int rjlog_init(const char *comName);

void _rj_log(const char *tag, const char *file, const char *func, int line, int level, const char *fmt, ...);

#endif /*__RJ_LOG_H__*/
