#ifndef __RJ_COMMON_H__
#define __RJ_COMMON_H__

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "rj_log.h"
#include "rj_utils.h"
#include "rj_error.h"
#include "rj_math.h"
#include "sysrjcore_linux.h"
#include "network_extra.h"
#include "iniparser.h"
#include "rj_thread.h"

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)
#define MAX_LEN_16             16
#define SIZE_1K                1024
#define SIZE_512               512
#define SIZE_256               256
#define SIZE_128               128
#define SIZE_64                64

#define RJ_IDV_TYPE            "IDV"
#define RJ_VDI_TYPE            "VDI"
#define RJ_HDMI_AUDIO          "hdmi"
#define RJ_ANALOG_AUDIO        "analog"
#define RJ_LANG_CHINESE        "chinese"
#define RJ_LANG_ENGLISH        "english"
#define RJ_SCRIPT_PATH         "/usr/local/bin/system/"
#define RJ_SYS_CONFIG_PATH     "/etc/RainOScfg/"
#define RJ_USR_CONFIG_PATH     "/etc/UserCfg/"
extern "C" bool isInit();

#define CHECK_INIT_INT()                            \
    {                                               \
        if (isInit() == false) {                    \
            return ERROR_10001;                     \
        }                                           \
    }

#define CHECK_INIT_STR(buf, len)                    \
    {                                               \
        if (isInit() == false) {                    \
            if ((buf) != NULL) {                    \
                snprintf((buf), len, "%s", rj_strerror(ERROR_10001).c_str()); \
            }                                       \
            return ERROR_10001;                     \
        }                                           \
    }

#define CHECK_INIT_STR_EX(buf, len)                 \
    {                                               \
        if (isInit() == false) {                    \
            if ((buf) != NULL) {                    \
                snprintf((buf), len, "%s", rj_strerror(ERROR_10001).c_str()); \
            }                                       \
            return;                                 \
        }                                           \
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
            return ERROR_11003;                                   \
        }                                                         \
    } while (0)

#define CHECK_STR_PARA_STR(para, ret, size)                       \
    do {                                                          \
        if (para == NULL || *para == '\0') {                      \
            rjlog_error("the para is invalid");                   \
            snprintf((ret), size, "%s", rj_strerror(ERROR_11003).c_str()); \
            return ERROR_11003;                                   \
        }                                                         \
    } while (0)



#endif /*__RJ_COMMON_H__*/
