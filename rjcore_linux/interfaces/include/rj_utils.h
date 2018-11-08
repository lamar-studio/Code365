#ifndef __RJ_UTILS_H__
#define __RJ_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "rj_log.h"

int rj_system(const char *command);
size_t rj_strncp_line(char *dst, const char *src, size_t size);
int rj_exec_result(const char *cmd, char *result, size_t size);
int rj_exec(const char *cmd);

uint64_t rj_get_file_size(const char *path);
int rj_open_filelock(const char *lckname);
int rj_close_filelock(int fd);
int rj_filelock_ex(int fd);
int rj_filelock_unlock(int fd);

#endif /*__RJ_UTILS_H__*/
