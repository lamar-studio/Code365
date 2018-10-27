#define MODULE_TAG "rj_log"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include "rj_error.h"

#define MAX_BUF 64

string rj_strerror(int error)
{
    char buf[MAX_BUF] = {0};

    snprintf(buf, MAX_BUF, "RJFAIL%d", error);
    
    return buf;
}

