
/*
 * decs: test for sysabslayer_linux cpp interface
 *
 * make: gcc -o test xset_test.c -I/usr/loacl/include -L/usr/local/lib -lsysabslayer_linux
 * author: LaMar
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "sysabslayer_linux.h"

#define log(format, args...)  printf(format"\n", ##args)
#define mlog(format, args...)                                 \
    do {                                                      \
            if(DEBUG) printf(format"\n", ##args);             \
        } while(0)


int main(int argc, char **argv)
{
    int ret = 0;
    int s = 0;
    int cnt = 0;

    ret = InitForMisc("RainOS", NULL);
    if (ret != 0) {
        log("InitForMisc fail.");
        return -1;
    }

    while (1) {
        log("setSleeptime:%d, cnt:%d", s, cnt);
        ret = setSleeptime(s);
        if (ret < 0) {
            log("setSleeptime, fail:%d", ret);
        }
        s = s + 10;
        if (s > 3600) {
            s = 0;
        }
        cnt++;
        usleep(100*1000);
    }

    return 0;
}




























