#ifndef __RJ_COMMON_H__
#define __RJ_COMMON_H__

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "rj_log.h"
#include "rj_utils.h"
#include "rj_error.h"
#include "sysrjcore_linux.h"
#include "hw_config.h"
#include "network_extra.h"

extern callback g_callback;
extern bool g_isinit;

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)
#define MAX_LEN_16 16
#define RJ_SCRIPT_PATH         "/usr/local/bin/system/"
#define RJ_SYS_CONFIG_PATH     "/etc/RainOScfg/"
#define RJ_USR_CONFIG_PATH     "/etc/UserCfg/"

#define CHECK_INIT_INT()\
    {\
        if (g_isinit == false) {\
            return ERROR_10001;\
        }\
    }

#define CHECK_INIT_STR(buf, len)  \
    {\
        if (g_isinit == false) {\
            if ((buf) != NULL) {\
                snprintf((buf), len, "%s", rj_strerror(ERROR_10001).c_str()); \
            }\
            return ERROR_10001; \
        }\
    }

#define CHECK_FUNCTION_IN()                                       \
    do {                                                          \
        rjlog_info("== IN ==");                                   \
    } while (0)

#define CHECK_FUNCTION_OUT()                                      \
    do {                                                          \
        rjlog_info("== OUT ==");                                  \
    } while (0)

#define CHECK_STR_PARA(para)                                      \
    do {                                                          \
        if (para == NULL || *para == '\0') {                      \
            rjlog_error("the para is invalid");                   \
            return ERROR_10001;                                   \
        }                                                         \
    } while (0)



#endif /*__RJ_COMMON_H__*/
