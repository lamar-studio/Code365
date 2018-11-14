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

#define SYSTEM_CMDLINE_LENGTH   1024

int rj_system_rw(const char *command, unsigned char *data, int *size, const char* mode) {
        int readsize = 0, n;
        FILE *fd;

        if ((NULL != data) && (NULL != size)) {
                readsize = *size;
                *size = 0;
        }

        fd = rj_popen(command, mode);
        if (NULL != fd) {
                if (*mode == 'r') {
                        while ((readsize > 0) && ((n = fread(data, 1, readsize, fd)) > 0)) {
                                *size += n;
                                readsize -= n;
                                data += n;
                        }
                } else {
                        while ((readsize > 0) && ((n = fwrite(data, 1, readsize, fd)) > 0)) {
                                *size += n;
                                readsize -= n;
                                data += n;
                        }
                }
                return rj_pclose(fd);
        }
        rjlog_error("rj_popen failed");

        return -1;
}

int rj_system_r_args(char *rbuf, int rbuf_size, const char *fmt, ...)
{
    char cmd[SYSTEM_CMDLINE_LENGTH];
    int cmdl = 0;
    int ret = 0;
    int rdsize = 0;
    va_list args;
    if ((NULL != rbuf) && (rbuf_size > 0))
    {
        *rbuf = '\0';
        rdsize = rbuf_size;
    }
    va_start(args, fmt);
    cmdl = vsnprintf(cmd, sizeof(cmd), fmt, args);
    va_end(args);
    if (cmdl >= SYSTEM_CMDLINE_LENGTH) {
        rjlog_info("cmd string to long.");
        return -1;
    } else if (cmdl > 0) {
        rjlog_info("run cmd:%s", cmd);
        if (rdsize)
        {
            ret = rj_system_rw(cmd, (unsigned char *)rbuf, &rdsize, "r");
            if (rdsize >= rbuf_size){
                rdsize = rbuf_size - 1;
            }
            rbuf[rdsize] = '\0';
            return ret;
        }else {
            return rj_system(cmd);
        }
    } else {
        rjlog_info("cmd string invalue.");
        return -1;
    }
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

int rj_system_args(const char *fmt, ...)
{
    char cmd[SYSTEM_CMDLINE_LENGTH];
    int cmdl = 0;
    va_list args;
    va_start(args, fmt);
    cmdl = vsnprintf(cmd, sizeof(cmd), fmt, args);
    va_end(args);
    if (cmdl >= SYSTEM_CMDLINE_LENGTH) {
        rjlog_info("cmd string to long.");
        return -1;
    } else if (cmdl > 0) {
        rjlog_info("run cmd:%s", cmd);
        return rj_system(cmd);
    } else {
        rjlog_info("cmd string invalue.");
        return -1;
    }
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

int dev_exec(const char *cmd, vector<string> &resvec)
{

    FILE *pp = popen(cmd, "r");

    if (!pp) {
        rjlog_error("popen failed");
        return -1;
    }

    char tmp[1024];
    resvec.clear();

    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0';
        }
        resvec.push_back(tmp);
    }

    pclose(pp);

    return resvec.size();
}

int check_ipaddr(const char *str)
{
    struct sockaddr_in6 addr6;
    struct sockaddr_in addr4;

    if (str == NULL || *str == '\0') {
        rjlog_error("the str is invalid.");
        return 1;
    }

    if (inet_pton(AF_INET, str, &addr4.sin_addr) == 1) {
        return 0;
    } else if (inet_pton(AF_INET6, str, &addr6.sin6_addr) == 1) {
        return 0;
    }

    return 1;
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
