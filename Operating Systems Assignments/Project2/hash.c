#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

// Function Prototypes Needed for Implementation

void init_locks(sem_t *locks, int K);
void delete_locks(sem_t *locks, int K);

// Function Implementations

HashTable *hash_init(int N, int K)
{
    h_item **tmp = calloc(N, sizeof(h_item*));
    // Check for success
    if (!tmp) {
        return NULL;
    }

    sem_t *tmp2 = calloc(K, sizeof(sem_t));
    // Check for success
    if (!tmp2) {
        free(tmp2);
        return NULL;
    }

    HashTable *hp = calloc(1, sizeof(HashTable));
    // Check for success
    if (!hp) {
        free(tmp);
        return NULL;
    }

    hp->size = N;
    hp->num_keys = K;
    hp->reg_size = N / K;
    hp->buckets = tmp;
    hp->locks = tmp2;

    init_locks(hp->locks, hp->num_keys);

    return hp;
}

void init_locks(sem_t *locks, int K) {
    int i;

    for (i = 0; i < K; i++) {
        sem_init(&locks[i], 0, 1);
    }
}

int hash_insert(HashTable *hp, int k, void *v)
{
    // Check valid key
    if (k <= 0) {
        return -1;
    }

    // Find appropriate bucket
    int bucket_ind = k % hp->size;

    // acquire lock
    sem_t* tempSem = &(hp->locks[bucket_ind / hp->reg_size]);
    sem_wait(tempSem);

    // Check if key exists
    h_item *tmp = hp->buckets[bucket_ind];
    while (tmp != NULL) {
        if (tmp->key == k) {
            // release lock
            sem_post(tempSem);
            return -1;
        }
        tmp = tmp->next;
    }

    // Create record reusing temporary pointer
    tmp = calloc(1, sizeof(h_item));
    // Check for success
    if (!tmp) {
        // release lock
        sem_post(tempSem);
        return -1;
    }
    tmp->key = k;
    tmp->value = v;

    // Insert to head of the bucket - CRITICAL SECTION
    tmp->next = hp->buckets[bucket_ind];
    hp->buckets[bucket_ind] = tmp;

    // release lock
    sem_post(tempSem);
    return 0;
}

int hash_delete(HashTable *hp, int k)
{
    // Check valid key
    if (k <= 0) {
        return -1;
    }

    // Search for the key
    int bucket_ind = k % hp->size;
    h_item *tmp = hp->buckets[bucket_ind];

    // acquire lock
    sem_t* tempSem = &(hp->locks[bucket_ind / hp->reg_size]);
    sem_wait(tempSem);

    // If list is empty
    if (tmp == NULL) {
        // release lock
        sem_post(tempSem);
        return -1;
    }
    // If head has the key
    else if (tmp->key == k) {
        hp->buckets[bucket_ind] = tmp->next;
        tmp->value = NULL;

        // release lock
        sem_post(tempSem);
        return 0;
    }
    // Search through the list
    else {
        while (tmp->next != NULL) {
            if (tmp->next->key == k) {
                h_item *del = tmp->next;
                tmp->next = del->next;
                tmp->value = NULL;

                // release lock
                sem_post(tempSem);
                return 0;
            }
            tmp = tmp->next;
        }

        // release lock
        sem_post(tempSem);
        return -1;
    }
}

int hash_update(HashTable *hp, int k, void *vp)
{
    // Check valid key
    if (k <= 0) {
        return -1;
    }

    // Search for the key
    int bucket_ind = k % hp->size;
    h_item *tmp = hp->buckets[bucket_ind];

    // acquire lock
    sem_t* tempSem = &(hp->locks[bucket_ind / hp->reg_size]);
    sem_wait(tempSem);

    // If list is empty
    if (tmp == NULL) {
        // release lock
        sem_post(tempSem);
        return -1;
    }
    // Search through the list
    else {
        while (tmp != NULL) {
            if (tmp->key == k) {
                tmp->value = vp;
                // release lock
                sem_post(tempSem);
                return 0;
            }
            tmp = tmp->next;
        }
        // release lock
        sem_post(tempSem);
        return -1;
    }
}

int hash_get(HashTable *hp, int k, void **vp)
{
    // Check valid key
    if (k <= 0) {
	return -1;
    }

    // Find appropriate bucket
    int bucket_ind = k % hp->size;
    h_item *tmp = hp->buckets[bucket_ind];

    // acquire lock
    sem_t* tempSem = &(hp->locks[bucket_ind / hp->reg_size]);
    sem_wait(tempSem);

    // If list is empty
    if (tmp == NULL) {
        // release lock
        sem_post(tempSem);
        return -1;
    }
    // Search through the list
    else {
        while (tmp != NULL) {
            if (tmp->key == k) {
                *vp = tmp->value;//*vp = tmp;

                // release lock
                sem_post(tempSem);

                return 0;
            }
            tmp = tmp->next;
        }
        // release lock
        sem_post(tempSem);

        return -1;
    }
}

int hash_destroy(HashTable *hp)
{
    // Check valid table
    if (hp == NULL) {
	return -1;
    }

    // Delete buckets
    h_item *tmp;
    h_item *prev;
    for (int i = 0; i < hp->size; i++) {
        tmp = hp->buckets[i];
        hp->buckets[i] = NULL;
        while (tmp != NULL) {
            prev = tmp;
            tmp = tmp->next;
            free(prev);
        }
    }
    free(hp->buckets);

    // Delete the array that holds the locks
    delete_locks(hp->locks, hp->num_keys);
    free(hp->locks);

    // Delete the hash table CHANGE WAS HERE
    free(hp);

    return 0;
}

void delete_locks(sem_t *locks, int K) {
    int i;

    for (i = 0; i < K; i++) {
        sem_destroy(&locks[i]);
    }
}