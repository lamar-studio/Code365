#ifndef __RJ_UTILS_H__
#define __RJ_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <string>
#include <arpa/inet.h>

#include "rj_log.h"

using std::string;
using std::vector;

int rj_system_rw(const char *command, unsigned char *data, int *size, const char* mode);
int rj_system_r_args(char *rbuf, int rbuf_size, const char *fmt, ...);
int rj_system(const char *command);
int rj_system_args(const char *fmt, ...);
size_t rj_strncp_line(char *dst, const char *src, size_t size);
int rj_exec_result(const char *cmd, char *result, size_t size);
int rj_exec(const char *cmd);
int dev_exec(const char *cmd, vector<string> &resvec);
int check_ipaddr(const char *str);

uint64_t rj_get_file_size(const char *path);
int rj_open_filelock(const char *lckname);
int rj_close_filelock(int fd);
int rj_filelock_ex(int fd);
int rj_filelock_unlock(int fd);

#endif /*__RJ_UTILS_H__*/
