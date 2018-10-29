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
	if (NULL == command) {
		rjlog_error("command is null");
		return NULL;
	}
	if (NULL == mode) {
		rjlog_error("mode is not set");
		return NULL;
	}
	errno = 0;
	fp = popen(command, mode);
	if (NULL == fp) {
		rjlog_error("popen failed");
		if (0 != errno) {
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
	if (NULL == fp){
		rjlog_error("invalid ptr");
		return -1;
	}
	errno = 0;
	ret = pclose(fp);
	if (ret < 0){
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
 * Function: rj_system_rw
 * Description: run system cmd with only read or write mode
 * Input: 	@command
 * 			@mode	"r" or "w"
 * In&Out:	@data	data input or output, NULL when none data
 * 			@size	the size of data to read or write
 * Return:	-1 	failed to run cmd
 * 			>=0 cmd return value
 */
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

/**
 * Function: rj_system
 * Description: run system cmd without read or write mode
 * Input: 	@command
 * Return:	-1 	failed to run cmd
 * 			>=0 cmd return value
 */
int rj_system(const char *command) {
	FILE *fd;
	fd = rj_popen(command,"w");
	if (NULL != fd) {
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
int rj_strncp_line(char *dst, const char *src, int size) {
	int cpsize = 0;
	while (size-- > 0) {
		if ((0 == *src) || (*src < 0x20) || (*src == '\n') || (*src == '\r')) {
            break;
        }
		*dst++ = *src++;
		cpsize++;
	}
	return cpsize;
}


int rj_exec_result(char *cmd, char *line_buf, int size)
{
    FILE *fp;
    char *last_char;

    fp = popen(cmd,"r");
    if(fp == NULL)
    {
    	rjlog_error("cmd %s err(%s).\n", cmd, strerror(errno));
        return -1;
    }

    if (line_buf != NULL) {
    	memset(line_buf, 0, size);
    	if (fgets(line_buf, size-1, fp) == NULL) {
    		goto end;
    	}
    	if (strlen(line_buf) > 0) {
    		last_char = line_buf + strlen(line_buf) - 1;
    		if (*last_char == '\n' || *last_char == '\r') {
    			*last_char = '\0';
    		}
    	}
    }
end:
	pclose(fp);
	return 0;
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

