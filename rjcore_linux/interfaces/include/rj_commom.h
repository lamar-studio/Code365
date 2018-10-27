#ifndef __RJ_COMMON_H__
#define __RJ_COMMON_H__

#include <stddef.h>
#include <stdint.h>
#include "rj_log.h"
#include "rj_utils.h"
#include "rj_error.h"
#include "sysrjcore_linux.h"

extern callback g_callback;
extern bool g_isinit;

#define CHECK_INIT_INT()\
    {\
        if (g_isinit == false) {\
            return ERROR_10001;\
        }\
    }

#define CHECK_INIT_STR(buf)  \
    {\
        if (g_isinit == false) {\
            if ((buf) != NULL) {\
                strcpy((buf), rj_strerror(ERROR_10001).c_str()); \
            }\
            return ERROR_10001; \
        }\
    }

#endif /*__RJ_COMMON_H__*/
