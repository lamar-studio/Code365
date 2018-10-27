#define MODULE_TAG "rj_utils"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

#define BUF_MAX 1024

int rj_exec(const char *cmd)
{
    FILE *pp = popen(cmd, "r");
    if (!pp) {
        return -1;
    }

    pclose(pp);

    return 0;
}

uint64_t rj_get_file_size(const char *path)
{
    uint64_t filesize = 0;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0) {
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
}

int rj_open_filelock(const char *lckname)
{   
    char lckpath[BUF_MAX] = {0};
    int fd;
    
    snprintf(lckpath, BUF_MAX, "%s/%s", "/tmp", lckname);
    
    fd = open(lckpath, O_WRONLY|O_CREAT, S_IRWXU);
    if (fd < 0) {
        return -1;
    }

    return fd;
}

int rj_close_filelock(int fd)
{
    return close(fd);
}

int rj_filelock_ex(int fd)
{
    int retry;
    int ret = -1;

    retry = 30000;

    while (retry--) {
        ret = flock(fd, LOCK_EX | LOCK_NB);
        if (ret == 0) {
            return 0;
        }
        usleep(1000);
    }
    return ret;
}

int rj_filelock_unlock(int fd)
{
    return flock(fd, LOCK_UN);
}

