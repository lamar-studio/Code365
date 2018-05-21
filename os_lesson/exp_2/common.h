#ifndef __COMMON_H__
#define __COMMON_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>


struct work_record {
	int tag;
	int p_count;
	int c_count;
};

#endif