
#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/wait.h>

#define SIZE_1K                1024
#define SIZE_512               512
#define SIZE_256               256
#define SIZE_128               128
#define SIZE_64                64
#define SIZE_32                32
#define SIZE_16                16

int exec_result(const char *cmd, char *result, size_t size);
int rj_system(const char *command);

#endif

