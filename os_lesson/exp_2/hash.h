#ifndef HASH_H
#define HASH_H
#include "common.h"


typedef struct hash_table Hash;


Hash *hash_new(void);
void hash_delete(Hash *self);


void hash_set(Hash **selfp, const char *key, const struct work_record value);
int hash_get(const Hash *self, const char *key, struct work_record *value_out);
void hash_remove(Hash **selfp, const char *key);
void hash_foreach(Hash *self);


#endif