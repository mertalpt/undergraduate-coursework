#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

// Libraries

#include <semaphore.h>

// Definitions

#define MIN_N 100
#define MAX_N 1000
#define MIN_M 10
#define MAX_M 1000

// Structs

typedef struct h_item {
	int key;
	void *value;
	struct h_item *next;
} h_item;

typedef struct hash_table {
	int size;
	int num_keys;
	int reg_size;
	h_item **buckets;
	sem_t *locks;
} hash_table;

typedef struct hash_table HashTable;

// Functions

HashTable *hash_init(int N, int K);
int hash_insert(HashTable *hp, int k, void *v);
int hash_delete(HashTable *hp, int k);
int hash_update(HashTable *hp, int k, void *vp);
int hash_get(HashTable *hp, int k, void **vp);
int hash_destroy(HashTable *hp);

#endif // Define __HASH_TABLE_H_
