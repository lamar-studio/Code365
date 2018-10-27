#ifndef __HASH_H__
#define __HASH_H__

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

#include "server.h"


typedef struct hash_table Hash;


Hash *hash_new(void);
void hash_delete(Hash *self);


void hash_set(Hash **selfp, const char *key, const struct server_info value);
int hash_get(const Hash *self, const char *key, struct server_info *value_out);
void hash_remove(Hash **selfp, const char *key);
void hash_foreach(Hash *self);


#endif
