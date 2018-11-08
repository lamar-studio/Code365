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

#include "rj_utils.h"

#define BUF_MAX 1024

/**
 * Function: rj_popen
 * Description: same as popen, and auto write log
 */
static FILE *rj_popen( const char* command, const char* mode ) {
	FILE *fp = NULL;

	if (command == NULL) {
		rjlog_error("command is null");
		return NULL;
	}
	if (mode == NULL) {
		rjlog_error("mode is not set");
		return NULL;
	}
	errno = 0;
	fp = popen(command, mode);
	if (fp == NULL) {
		rjlog_error("popen failed");
		if (errno != 0) {
            rjlog_error("errno:%d: %s", errno, strerror(errno));
		}
		return NULL;
	}
	return fp;
}

/**
 * Function: rj_pclose
 * Description: same as pclose, and auto write log
 */
static int rj_pclose( FILE *fp ) {
	int ret;
	if (fp == NULL){
		rjlog_error("invalid ptr");
		return -1;
	}
	errno = 0;
	ret = pclose(fp);
	if (ret < 0) {
        rjlog_error("errno:%d: %s", errno, strerror(errno));
		return ret;
	}

	if (errno) {
        rjlog_error("errno:%d: %s", errno, strerror(errno));
    }

	if (WIFSIGNALED(ret)) {
		rjlog_error("popen command abnormal,signal number:%d %s", WTERMSIG(ret), strsignal(WTERMSIG(ret)));
		return -1;
	} else if (WIFSTOPPED(ret)) {
		rjlog_error("popen command stop,signal number:%d %s", WSTOPSIG(ret), strsignal(WSTOPSIG(ret)));
		return -1;
	} else if (WIFEXITED(ret)) {
		ret = WEXITSTATUS(ret);
		if (ret) {
            rjlog_error("popen command exit status:%d", ret);
        }
	}

	return ret;
}


/**
 * Function: rj_system
 * Description: run system cmd without read or write mode
 * Input: 	@command
 * Return:	-1 	failed to run cmd
 * 			>=0 cmd return value
 */
int rj_system(const char *command) {
	FILE *fd;

	fd = rj_popen(command, "w");
	if (fd != NULL) {
		return rj_pclose(fd);
	}

	rjlog_error("rj_popen failed");
	return -1;
}

/**
 * Function: rj_strncp_line
 * Description: copy one line string that not more than size, don't copy '\n' and '\r'
 * Input:	dst copy to
 * 			src copy from
 * 			size copy max size
 * 	Return: the size copied
 */
size_t rj_strncp_line(char *dst, const char *src, size_t size) {
	size_t cpsize = 0;

	while (size-- > 0) {
		if ((*src < 0x20) || (*src == '\n') || (*src == '\r')) {
            break;
        }
		*dst++ = *src++;
		cpsize++;
	}

	return cpsize;
}

int rj_exec_result(const char *cmd, char *result, size_t size) {
    if (cmd == NULL || *cmd == '\0' || result == NULL) {
        rjlog_error("the cmd is invalid");
        return -1;
    }

    char *pBuf = result;
    int len = 0;
    int ret = 0;
    FILE *ptr = NULL;

    if ((ptr = rj_popen(cmd, "r")) != NULL) {
        while (fgets(pBuf, size, ptr) != NULL) {
            if ((len = strlen(pBuf)) == 0) {
                rjlog_warn("the result is full");
                break;
            }

            size -= len;
            pBuf += len;
        }

        ret = rj_pclose(ptr);
        ptr = NULL;
    } else {
        rjlog_error("open %s err:%s", cmd, strerror(errno));
        return -1;
    }

    return ret;
}

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

/*
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
*/
