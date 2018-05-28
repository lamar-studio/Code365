

#include "hash.h"


#define MIN_BUCKET_COUNT 100

#define GROW_THRESHOLD 1.5
#define SHRINK_THRESHOLD (GROW_THRESHOLD / 4.0)



struct hash_entry {
	char *key;
	struct server_info value;
	struct hash_entry *next;
};

struct hash_table {
	double load_factor;
	size_t bucket_count;
	struct hash_entry *buckets[];
};

static unsigned long hash(const char *key)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *key++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

static Hash *make_hash(size_t bucket_count)
{
	Hash *h = malloc(sizeof *h + bucket_count * sizeof h->buckets[0]);
	if (!h) return NULL;

	h->load_factor = 0.0;
	h->bucket_count = bucket_count;

	for (size_t i = 0; i < h->bucket_count; i++) {
		h->buckets[i] = NULL;
	}

	return h;
}

Hash *hash_new()
{
	return make_hash(MIN_BUCKET_COUNT);
}

static void delete_bucket(struct hash_entry *head)
{
	struct hash_entry *next;

	while (head) {
		next = head->next;
		free(head->key);
		free(head);
		head = next;
	}
}

void hash_delete(Hash *self)
{
	if (!self) return;

	for (size_t i = 0; i < self->bucket_count; i++) {
		delete_bucket(self->buckets[i]);
	}

	free(self);
}

static struct hash_entry * make_entry(const char *key, const struct server_info value)
{
	struct hash_entry *p = NULL;

	p = malloc(sizeof *p);
	if (p == NULL) goto error;

	p->key = malloc(strlen(key) + 1);
	if (p->key == NULL) goto error;

	strcpy(p->key, key);
	p->value = value;
	p->next = NULL;

	return p;

error:
	free(p);
	return NULL;
}

static int try_set(Hash *self, const char *key, const struct server_info value)
{
	size_t i = hash(key) % self->bucket_count;
	struct hash_entry *entry;

	/* Search for key */
	entry = self->buckets[i];
	while (entry && strcmp(key, entry->key) !=0)
		entry = entry->next;

	if (entry) {
		/* Key found */
		entry->value = value;
	} else {
		/* Key not found */
		entry = make_entry(key, value);
		if (!entry) return 0;

		entry->next = self->buckets[i];
		self->buckets[i] = entry;

		self->load_factor += 1.0 / self->bucket_count;
	}

	return 1;
}

void hash_set(Hash **selfp, const char *key, const struct server_info value)
{
	assert(*selfp);
	assert(key);

	Hash *self = *selfp;

	if (!try_set(self, key, value)) goto error;

	return;

error:
	abort();
}

int hash_get(const Hash *self, const char *key, struct server_info *value_out)
{
	assert(self);
	assert(key);

	size_t i = hash(key) % self->bucket_count;
	struct hash_entry *entry;

	entry = self->buckets[i];
	while (entry && strcmp(key, entry->key) != 0)
		entry = entry->next;

	if (entry) {
		/* Key found */
		if (value_out) *value_out = entry->value;
		return 1;
	} else {
		/* Key not found */
		return 0;
	}
}


void hash_remove(Hash **selfp, const char *key)
{
	assert(*selfp);
	assert(key);

	Hash *self = *selfp;

	size_t i = hash(key) % self->bucket_count;
	struct hash_entry **entryp;


	entryp = &self->buckets[i];
	while (*entryp && strcmp(key, (*entryp)->key) != 0) {
		entryp = &(*entryp)->next;
	}

	if (*entryp) {
		/* Key found */
		struct hash_entry *entry = *entryp;

		*entryp = entry->next;

		free(entry->key);
		free(entry);

		self->load_factor -= 1.0 / self->bucket_count;
	}

	return;
}

void hash_foreach(Hash *self)
{
#if 0
	int num = 0;
	for(size_t i = 0; i < self->bucket_count; i++)
		for(struct hash_entry *e = self->buckets[i]; e; e = e->next) {
			num++;
			printf("e->value.tag = %d e->value.p_count = %d e->value.c_count = %d\n",e->value.tag,e->value.p_count,e->value.c_count);
		}
	printf("the product num = %d\n",num);
#endif
}

