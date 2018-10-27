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

int bt()
{
    CHECK_INIT_INT();
    rjlog_info("function enter.");

    g_callback("bt", 1, "bt_callback");

    rjlog_info("function exit.");
    return 0;
} 

#ifdef __cplusplus
}
#endif
