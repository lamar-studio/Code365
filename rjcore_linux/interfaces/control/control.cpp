#define MODULE_TAG    "control"
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

int control(const char *arg, char *retbuf)
{
    CHECK_INIT_STR(retbuf);
    rjlog_info("function enter.");

    g_callback("control", 1, "control_callback");
    strcpy(retbuf, "Hello, ");
    strcat(retbuf, arg);

    rjlog_info("function exit.");
  
    return 0;
} 

#ifdef __cplusplus
}
#endif
