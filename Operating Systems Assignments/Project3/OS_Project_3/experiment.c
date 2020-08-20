#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "pthread.h"
#include "ralloc.h"

int handling_method; 		// deadlock handling method

#define M 3					// number of resource types
int exist[3] = {100, 100, 100}; // resources existing in the system

#define N 10 				// number of processes - threads
pthread_t tids[N];			// ids of created threads

#define K 10				// ratio of exist to maximum value max can get for a thread

int finished[N];            // to indicate threads have finished
pthread_mutex_t lock;       // to control access to finished

void *aprocess (void *p)
{
    int max[3];
	int req[3];
    int tmp[3];
	int k;
	int pid;

	pid = *((int *)p);

	max[0] = (rand() % exist[0]) / K + 1;
	max[1] = (rand() % exist[1]) / K + 1;
	max[2] = (rand() % exist[2]) / K + 1;

	ralloc_maxdemand(pid, max);

	for (k = 0; k < 10; ++k) {
        req[0] = rand() % max[0] + 1;
        req[1] = rand() % max[1] + 1;
        req[2] = rand() % max[2] + 1;
        
        if (req[0] != max[0]) {
            tmp[0] = rand() % (max[0] - req[0]) + 1;
        }
        else {
            req[0]--;
            tmp[0] = 1;
        }

        if (req[1] != max[1]) {
            tmp[1] = rand() % (max[1] - req[1]) + 1;
        }
        else {
            req[1]--;
            tmp[1] = 1;
        }

        if (req[2] != max[2]) {
            tmp[2] = rand() % (max[2] - req[2]) + 1;
        }
        else {
            req[2]--;
            tmp[2] = 1;
        }
		
		ralloc_request(pid, req);
        ralloc_request(pid, tmp);

		int i;
		for (i = 0; i < 1000; i++) {
			// Waste time
		}

		ralloc_release(pid, req);
        ralloc_release(pid, tmp);
	}

    pthread_mutex_lock(&lock);
    finished[pid] = true;
    pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

void test()
{
    int dn; // number of deadlocked processes
	int deadlocked[N]; // array indicating deadlocked processes
	int k;
	int i;
	int pids[N];

	for (k = 0; k < N; ++k)
		deadlocked[k] = -1; // initialize
	
	ralloc_init(N, M, exist, handling_method);

	for (i = 0; i < N; ++i) {
		pids[i] = i;
		pthread_create(&(tids[i]), NULL, (void *) &aprocess,
						(void *)&(pids[i]));
	}

	i = 0;
	while (1) {
		
		if (handling_method == DEADLOCK_DETECTION) {
			dn = ralloc_detection(deadlocked);
			if (dn > 0) {
				// Force exit
				for (i = 0; i < N; i++) {
					pthread_cancel(tids[i]);
				}
				break;
			}
		}
		
        // Break if all threads have finished
        int all_finished = true;
        pthread_mutex_lock(&lock);
        for (i = 0; i < N; i++) {
            if (finished[i] == false) {
                all_finished = false;
            }
        }
        pthread_mutex_unlock(&lock);
        if (all_finished) {
            break;
        }
	}

	ralloc_end();
}

int main(int argc, char **argv)
{
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Error! Could not create a lock.\n");
        return -1;
    }

    int i;
	struct timeval tv1, tv2;

	for (i = 0; i < N; i++) {
        finished[i] = false;
    }

    printf("--- Starting test with handling method: DEADLOCK_AVOIDANCE ---\n");
    handling_method = DEADLOCK_AVOIDANCE;
    gettimeofday(&tv1, NULL);
    test();
	gettimeofday(&tv2, NULL);
	printf("Test took %ld microseconds.\n", (tv2.tv_sec - tv1.tv_sec) * 1000000000 + tv2.tv_usec - tv1.tv_usec);

	for (i = 0; i < N; i++) {
        finished[i] = false;
    }

    printf("--- Starting test with handling method: DEADLOCK_NOTHING ---\n");
    handling_method = DEADLOCK_NOTHING;
	gettimeofday(&tv1, NULL);
    test();
	gettimeofday(&tv2, NULL);
	printf("Test took %ld microseconds.\n", (tv2.tv_sec - tv1.tv_sec) * 1000000000 + tv2.tv_usec - tv1.tv_usec);

    for (i = 0; i < N; i++) {
        finished[i] = false;
    }

    printf("--- Starting test with handling method: DEADLOCK_DETECTION ---\n");
	handling_method = DEADLOCK_DETECTION;
    gettimeofday(&tv1, NULL);
    test();
	gettimeofday(&tv2, NULL);
	printf("Test took %ld microseconds.\n", (tv2.tv_sec - tv1.tv_sec) * 1000000000 + tv2.tv_usec - tv1.tv_usec);

    // Done
    pthread_mutex_destroy(&lock);
	return 0;
}