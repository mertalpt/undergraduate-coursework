#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "hash.h"

// Globals
HashTable *hp;
pthread_t *threads;

/*
    Calculates time difference in microseconds
*/
int calculateTimeDifference(struct timeval start, struct timeval end) {
    const int TV_SEC_MULT_FACTOR = 1000000;
    return (end.tv_sec * TV_SEC_MULT_FACTOR + end.tv_usec) - (start.tv_sec * TV_SEC_MULT_FACTOR + start.tv_usec);
}

void *tableOperations(void *input) {
    int i;
    int half_W = ((int)input) / 2 + 1;
    int *keys = calloc(half_W, sizeof(int));
    int exists;
    void *val;

    for (i = 0; i < half_W; i++) {
        keys[i] = i + 1;
    }

    for (i = 0; i < half_W; i++) {
        hash_insert(hp, keys[i], (void *)1);
    }
    for (i = 0; i < half_W; i++) {
        hash_delete(hp, keys[i]);
    }

    free(keys);
    return NULL;
}

void experiment(int T, int K, int N, int W, struct timeval *start, struct timeval *end) {
    hp = hash_init(N, K);

    // Create threads
    threads = calloc(T, sizeof(pthread_t));

    gettimeofday(start, NULL);
    for (int i = 0; i < T; i++) {
	    pthread_create(&threads[i], NULL, tableOperations, (void*)W);
    }
    for (int i = 0; i < T; i++) {
	    pthread_join(threads[i], NULL);
    }
    gettimeofday(end, NULL);

    printf("T: %2d, K: %3d, N: %3d, W: %4d | Time: %8d\n", T, K, N, W, calculateTimeDifference(*start, *end));
    hash_destroy(hp);
    free(threads);
}

int main()
{
    struct timeval start, end;

    printf("Starting the experiments.\n");
    experiment(1, 1, 1000, 1000, &start, &end);
    experiment(2, 1, 1000, 500, &start, &end);
    experiment(5, 1, 1000, 200, &start, &end);
    experiment(10, 1, 1000, 100, &start, &end);
    printf("-----\n");
    experiment(1, 1, 1000, 5000, &start, &end);
    experiment(2, 1, 1000, 2500, &start, &end);
    experiment(5, 1, 1000, 1000, &start, &end);
    experiment(10, 1, 1000, 500, &start, &end);
    printf("-----\n");
    experiment(1, 25, 1000, 1000, &start, &end);
    experiment(2, 25, 1000, 333, &start, &end);
    experiment(5, 25, 1000, 200, &start, &end);
    experiment(10, 25, 1000, 100, &start, &end);
    printf("-----\n");
    experiment(1, 25, 1000, 5000, &start, &end);
    experiment(2, 25, 1000, 2500, &start, &end);
    experiment(5, 25, 1000, 1000, &start, &end);
    experiment(10, 25, 1000, 500, &start, &end);
    printf("-----\n");
    experiment(1, 50, 1000, 1000, &start, &end);
    experiment(2, 50, 1000, 500, &start, &end);
    experiment(5, 50, 1000, 200, &start, &end);
    experiment(10, 50, 1000, 100, &start, &end);
    printf("-----\n");
    experiment(1, 50, 1000, 5000, &start, &end);
    experiment(2, 50, 1000, 2500, &start, &end);
    experiment(5, 50, 1000, 1000, &start, &end);
    experiment(10, 50, 1000, 500, &start, &end);
    printf("-----\n");
    experiment(1, 100, 1000, 1000, &start, &end);
    experiment(2, 100, 1000, 500, &start, &end);
    experiment(5, 100, 1000, 200, &start, &end);
    experiment(10, 100, 1000, 100, &start, &end);
    printf("-----\n");
    experiment(1, 100, 1000, 5000, &start, &end);
    experiment(2, 100, 1000, 2500, &start, &end);
    experiment(5, 100, 1000, 1000, &start, &end);
    experiment(10, 100, 1000, 500, &start, &end);
    printf("-----\n");
    experiment(1, 100, 100, 1000, &start, &end);
    experiment(2, 100, 100, 500, &start, &end);
    experiment(5, 100, 100, 200, &start, &end);
    experiment(10, 100, 100, 100, &start, &end);
    printf("-----\n");
    printf("End of the experiments.\n");

    return 0;
}