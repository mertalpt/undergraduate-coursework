#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "pthread.h"
#include "ralloc.h"

/*
    Internal function prototypes, structure definitions and globals
*/

// Prototoypes
int __is_safe();
void __free_handler();

// Structures
typedef struct deadlockHandlerData {
    int handlingMethod;
    int p_count;
    int r_count;
    int *r_exist;
    int *r_available;
    int **r_max;
    int **r_allocation;
    int **r_need;
    int **r_request;
} __dlock_handler;

// Global Variables
__dlock_handler *__handler;
pthread_mutex_t lock;
pthread_cond_t  cond;

/*
    Implementation of the library functions
*/

/*
    Purpose:
        Initializes necessary structures and variables in the library
        to do resource allocation and access control.
    Params:
        @p_count: Number of processes to control.
        @r_count: Number of resources to control.
        @r_exist: Array that holds the number of existing resources.
        @d_handling: Flag to indicate deadlock handling method.
    Returns:
        @ 0: Success.
        @-1: Failure.
*/
int ralloc_init(int p_count, int r_count, int r_exist[], int d_handling)
{
    // Check validity of arguments
    if (
        p_count < 0 || p_count > MAX_PROCESSES || r_count < 0 || r_count > MAX_RESOURCE_TYPES || r_exist == NULL ||
        (d_handling != DEADLOCK_NOTHING && d_handling != DEADLOCK_DETECTION && d_handling != DEADLOCK_AVOIDANCE)
        ) {
        perror("ralloc_init: Invalid arguments.\n");
        return -1;
    }

    __handler = NULL; // Must be initialized for error handling later.
    lock;
    cond;

    // Create lock
    if (pthread_mutex_init(&lock, NULL) != 0)
	{
		perror("ralloc_init: Semaphore initialization failed.\n");
		return -1;
	}

    // Create condition variable
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("ralloc_init: Condition variable initialization failed.\n");
        pthread_mutex_destroy(&lock);
        return -1;
    }

    /*
        Initialize data structures
    */
    int i; // Used throughout the function as loop index

    // Create deadlock handler
    __handler = calloc(1, sizeof(__dlock_handler));
    if (!__handler) {
        goto __init_fail;
    }
    else {
        __handler->handlingMethod = d_handling;
        __handler->p_count = p_count;
        __handler->r_count = r_count;
    }

    // Create exists array
    int *tmpR_exists = calloc(r_count, sizeof(int));
    if (!tmpR_exists) {
        goto __init_fail;
    }
    else {
        __handler->r_exist = tmpR_exists;
    }

    // Create available array
    int *tmpR_available = calloc(r_count, sizeof(int));
    if (!tmpR_available) {
        goto __init_fail;
    }
    else {
        __handler->r_available = tmpR_available;
    }

    // Create max array
    int **tmpR_max = calloc(p_count, sizeof(int*));
    if (!tmpR_max) {
        goto __init_fail;
    }
    else {
        __handler->r_max = tmpR_max;
        for (i = 0; i < p_count; i++) {
            int *tmp = calloc(r_count, sizeof(int));
            if (!tmp) {
                goto __init_fail;
            }
            else {
                tmpR_max[i] = tmp;
            }
        }
    }    

    // Create allocation array
    int **tmpR_allocation = calloc(p_count, sizeof(int*));
    if (!tmpR_allocation) {
        goto __init_fail;
    }
    else {
        __handler->r_allocation = tmpR_allocation;
        for (i = 0; i < p_count; i++) {
            int *tmp = calloc(r_count, sizeof(int));
            if (!tmp) {
                goto __init_fail;
            }
            else {
                tmpR_allocation[i] = tmp;
            }
        }
    }

    // Create need array
    int **tmpR_need = calloc(p_count, sizeof(int*));
    if (!tmpR_need) {
        goto __init_fail;
    }
    else {
        __handler->r_need = tmpR_need;
        for (i = 0; i < p_count; i++) {
            int *tmp = calloc(r_count, sizeof(int));
            if (!tmp) {
                goto __init_fail;
            }
            else {
                tmpR_need[i] = tmp;
            }
        }
    }    

    // Create request array
    int **tmpR_request = calloc(p_count, sizeof(int*));
    if (!tmpR_request) {
        goto __init_fail;
    }
    else {
        __handler->r_request = tmpR_request;
        for (i = 0; i < p_count; i++) {
            int *tmp = calloc(r_count, sizeof(int));
            if (!tmp) {
                goto __init_fail;
            }
            else {
                tmpR_request[i] = tmp;
            }
        }
    }    

    // Initialize the exists and available structures
    for (i = 0; i < r_count; i++) {
        __handler->r_exist[i] = r_exist[i];
        __handler->r_available[i] = r_exist[i];
    }

    // Successful initialization
    return 0;

    // Error handling target label
    __init_fail:
    perror("ralloc_init: Could not allocate memory for data.\n");
    __free_handler();
    return -1;
}

/*
    Purpose:
        Called by a thread before it uses the library to indicate the maximum number
        of resources it will demand to have at a time.
    Params:
        @pid: ID of the caller thread. Must be between 0 and p_count-1 inclusive.
        @r_max: Array of length p_count indicating demand for each resource type.
    Returns:
        @ 0: Success.
        @-1: Failure.
*/
int ralloc_maxdemand(int pid, int r_max[])
{
    // First check of validity of arguments
    if (__handler == NULL || pid < 0 || pid >= __handler->p_count || r_max == NULL) {
        goto __maxdemand_fail;
    }

    // Lock and get to work
    int i; // Used as loop index
    pthread_mutex_lock(&lock);

    // Second check if any demand is more than existing
    for (i = 0; i < __handler->r_count; i++) {
        if (r_max[i] > __handler->r_exist[i]) {
            pthread_mutex_unlock(&lock);
            goto __maxdemand_fail;
        }
    }

    // Initialize the max, need, allocation and request matrices
    for (i = 0; i < __handler->r_count; i++) {
        __handler->r_max[pid][i] = r_max[i];
        __handler->r_need[pid][i] = r_max[i];
    }

    // Unlock and indicate successful execution
    pthread_mutex_unlock(&lock);
    return 0;

    // Label for invalid argument failed return
    __maxdemand_fail:
    perror("ralloc_maxdemand: Invalid arguments.\n");
    return -1;
}

/*
    Purpose:
        Called by a thread to request resources from the library.
        If request cannot be fulfilled, the thread will be waited.
    Params:
        @pid: ID of the caller thread. Must be in range 0 to p_count-1 inclusive.
        @demand: Array of length p_count indicating request for each resource type.
    Returns:
        @ 0: Success.
        @-1: Failure.
*/
int ralloc_request(int pid, int demand[])
{
    // First check of validity of arguments
    if (__handler == NULL || pid < 0 || pid >= __handler->p_count || demand == NULL) {
        goto __request_fail;
    }

    // Lock and get to work
    int i; // Used as loop index
    pthread_mutex_lock(&lock);

    // Second check if any demand puts process over max demand
    for (i = 0; i < __handler->r_count; i++) {
        if (demand[i] + __handler->r_allocation[pid][i] > __handler->r_max[pid][i]) {
            pthread_mutex_unlock(&lock);
            goto __request_fail;
        }
    }

    // Handle request according to deadlock handling method
    switch (__handler->handlingMethod) {
        // Both cases are handled the same way, allocate if available
        case DEADLOCK_NOTHING:
        case DEADLOCK_DETECTION:
        // Register the demand as request
        for (i = 0; i < __handler->r_count; i++) {
            __handler->r_request[pid][i] += demand[i];
        }
        // Check if demand <= available
        __request_check_1:
        for (i = 0; i < __handler->r_count; i++) {
            while (demand[i] > __handler->r_available[i]) {
                pthread_cond_wait(&cond, &lock);
                goto __request_check_1; // Have to check from start
            }
        }
        // Allocate the resources
        for (i = 0; i < __handler->r_count; i++) {
            __handler->r_available[i] -= demand[i];
            __handler->r_allocation[pid][i] += demand[i];
            __handler->r_need[pid][i] -= demand[i];
            __handler->r_request[pid][i] -= demand[i];
        }
        break;

        // Checks if request is safe before granting the request
        case DEADLOCK_AVOIDANCE:
        // Check if demand <= available
        __request_check_2:
        for (i = 0; i < __handler->r_count; i++) {
            while (demand[i] > __handler->r_available[i]) {
                pthread_cond_wait(&cond, &lock);
                goto __request_check_2; // Have to check from start
            }
        }
        // Allocate the resources
        for (i = 0; i < __handler->r_count; i++) {
            __handler->r_available[i] -= demand[i];
            __handler->r_allocation[pid][i] += demand[i];
            __handler->r_need[pid][i] -= demand[i];
        }
        // Check if safe, if not loop until it is
        while (__is_safe(__handler->r_available, __handler->r_allocation, __handler->r_need) != 0) {
            // Unsafe, deallocate
            for (i = 0; i < __handler->r_count; i++) {
                __handler->r_available[i] += demand[i];
                __handler->r_allocation[pid][i] -= demand[i];
                __handler->r_need[pid][i] += demand[i];
            }
            // Wait until state changes to try again
            pthread_cond_wait(&cond, &lock);
            // Allocate and try again
            for (i = 0; i < __handler->r_count; i++) {
                __handler->r_available[i] -= demand[i];
                __handler->r_allocation[pid][i] += demand[i];
                __handler->r_need[pid][i] -= demand[i];
            }
        }
        break;
        
        default:
        perror("ralloc_request: Illegal deadlock handling method. Perhaps initialization went wrong?\n");
        pthread_mutex_unlock(&lock);
        return -1;
        break;
    }

    // Successful execution
    pthread_mutex_unlock(&lock);
    return 0;

    // Fail handling for invalid arguments
    __request_fail:
    perror("ralloc_request: Invalid arguments.\n");
    return -1;
}

/*
    Purpose:
        Called by a thread to release resources allocated to it.
        Library will check if any waiting thread can be waken up.
    Params:
        @pid: ID of the caller thread. Must be in range 0 to p_count-1 inclusive.
        @demand: Array of length p_count indicating amount of resources to release for each type.
    Returns:
        @ 0: Success.
        @-1: Failure.

*/
int ralloc_release(int pid, int demand[])
{
    // First check of validity of arguments
    if (__handler == NULL || pid < 0 || pid >= __handler->p_count || demand == NULL) {
        goto __release_fail;
    }

    // Lock and get to work
    int i; // Used as loop index
    pthread_mutex_lock(&lock);

    // Second check if demand <= allocated
    for (i = 0; i < __handler->r_count; i++) {
        if (demand[i] > __handler->r_allocation[pid][i]) {
            pthread_mutex_unlock(&lock);
            goto __release_fail;
        }
    }

    // Release the resources
    for (i = 0; i < __handler->r_count; i++) {
        __handler->r_available[i] += demand[i];
        __handler->r_allocation[pid][i] -= demand[i];
        __handler->r_need[pid][i] += demand[i];
    }

    // Wake up all waiting processes
    pthread_cond_broadcast(&cond);

    // Successful execution
    pthread_mutex_unlock(&lock);
    return 0;

    // Error handling for invalid arguments
    __release_fail:
    perror("ralloc_release: Invalid arguments.\n");
    return -1;
}

/*
    Purpose:
        Checks if there are any deadlocked threads at the moment.
    Params:
        @procarray: Output parameter, upon return deadlocked processes are marked 1
        and other processes are marked -1.
    Returns:
        Number of deadlocked processes.
*/
int ralloc_detection(int procarray[])
{
    // Check if function call is valid
    if (__handler->handlingMethod != DEADLOCK_DETECTION) {
        perror("ralloc_detection: Invalid call. Handling method is not detection.\n");
        return -1;
    }
    
    // Check if input is valid
    if (procarray == NULL) {
        perror("ralloc_detection: Invalid argument.\n");
        return -1;
    }

    // Acquire lock
    pthread_mutex_lock(&lock);
    int i, j; // Used as loop indices

    // Used in detection algorithm
    bool finish[__handler->p_count];
    int work[__handler->r_count];
    int available[__handler->r_count];
    int allocation[__handler->p_count][__handler->r_count];
    int request[__handler->p_count][__handler->r_count];

    // Copy available, allocation and request arrays.
    for (i = 0; i < __handler->r_count; i++) {
        available[i] = __handler->r_available[i];
        for (j = 0; j < __handler->p_count; j++) {
            allocation[j][i] = __handler->r_allocation[j][i];
            request[j][i] = __handler->r_request[j][i];
        }
    }

    // Initialize the finish array
    for (i = 0; i < __handler->p_count; i++) {
        finish[i] = true;
        for (j = 0; j < __handler->r_count; j++) {
            if (request[i][j] > 0) {
                finish[i] = false;
                break;
            }
        }
    }

    // Initialize the work array
    for (i = 0; i < __handler->r_count; i++) {
        work[i] = available[i];
    }

    for (i = 0; i < __handler->p_count; i++) {
        if (finish[i]) {
            for (j = 0; j < __handler->r_count; j++) {
                if (allocation[i][j] > 0) {
                    work[j] += allocation[i][j];
                }
            }
        }        
    }

    bool found = true;
    while (found) {
        found = false;
        int p;
        for (p = 0; p < __handler->p_count; p++)
        {
            // First check if a process is finished,
            // if not, go for next condition
            if (!finish[p])
            {
                // Check if request of current thread can be granted
                // Loop body is not needed for this, note the semicolon
                for (j = 0; j < __handler->r_count && request[p][j] <= work[j]; j++);

                // If true, request can be granted
                if (j == __handler->r_count)
                {
                    // Add the allocated resources of
                    // current P to the available/work
                    // resources i.e. free the resources
                    for (int k = 0 ; k < __handler->r_count; k++)
                        work[k] += allocation[p][k];

                    // Mark this p as finished
                    finish[p] = true;
                    found = true;
                }
            }
        }
    }

    // Copy finishable processes to output parameter and count deadlocked
    int count = 0;
    for (i = 0; i < __handler->p_count; i++) {
        if (finish[i] == true) {
            procarray[i] = -1;
        }
        else {
            procarray[i] = 1;
            count++;
        }
    }

    // Unlock and return
    pthread_mutex_unlock(&lock);
    return count;
}

/*
    Purpose:
        Does clean up if necessary.
    Params:
        None.
    Returns:
        @ 0: Success.
        @-1: Failure.
*/
int ralloc_end()
{
    // Acquire the lock to ensure threads cannot tamper with destruction
    pthread_mutex_lock(&lock);

    // Cleanup
    pthread_cond_destroy(&cond);
    __free_handler(); // Free data structures, checks for NULL inside

    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock); // Free mutex

    // Return
    return 0; // No idea how this could fail in a detectable manner
}

/*
    Implementation of the internal functions
*/

// Safe state detection algorithm.
int __is_safe(int *t_available, int **t_allocation, int **t_need) {
    int i;
    bool finish[__handler->p_count];
    int safeSeq[__handler->p_count];
    int work[__handler->r_count];
    int available[__handler->r_count];
    int allocation[__handler->p_count][__handler->r_count];
    int need[__handler->p_count][__handler->r_count];

    for (i = 0; i < __handler->r_count; i++) {
        int j;

        available[i] = t_available[i];
        for (j = 0; j < __handler->p_count; j++) {
            allocation[j][i] = t_allocation[j][i];
            need[j][i] = t_need[j][i];
        }
    }

    for (i = 0; i < __handler->p_count; i++) {
        finish[i] = false;
    }

    for (i = 0; i < __handler->r_count; i++) {
        work[i] = available[i];
    }

    int count = 0;
    while (count < __handler->p_count) {
        bool found = false;
        int p;
        for (p = 0; p < __handler->p_count; p++)
        {
            // First check if a process is finished,
            // if no, go for next condition
            if (!finish[p])
            {
                // Check if for all resources of
                // current P need is less
                // than work
                int j;
                for (j = 0; j < __handler->r_count; j++)
                    if (need[p][j] > work[j])
                        break;

                // If all needs of p were satisfied.
                if (j == __handler->r_count)
                {
                    // Add the allocated resources of
                    // current P to the available/work
                    // resources i.e.free the resources
                    for (int k = 0 ; k < __handler->r_count ; k++)
                        work[k] += allocation[p][k];

                    // Add this process to safe sequence.
                    safeSeq[count++] = p;

                    // Mark this p as finished
                    finish[p] = true;

                    found = true;
                }
            }
        }

        if (!found)
        {
            // Unsafe state
            return -1;
        }
    }

    // Safe state
    return 0;
}

// Frees library data structure, can be called at any stage of initialization
void __free_handler()
{
    // Check if handler is allocated at all
    if (__handler == NULL) {
        return;
    }

    /*
        HANDLER HOLDS:
            int handlingMethod;
            int p_count;
            int r_count;
            int *r_exist;
            int *r_available;
            int **r_max;
            int **r_allocation;
            int **r_need;
            int **r_request;
    */

    // Cleanup single arrays
    if (__handler->r_exist != NULL) { free(__handler->r_exist); }
    if (__handler->r_available != NULL) { free(__handler->r_available); }

    // Cleanup double arrays
    int i; // Used as loop index
    int p_count = __handler->p_count;

    /*
        For double arrays, allocation can fail at any single array
        Thus, check from inner to out and deallocate as necessary
    */

    // Max
    if (__handler->r_max != NULL) {
        for (i = 0; i < p_count; i++) {
            if (__handler->r_max[i] != NULL) {
                free(__handler->r_max[i]);
            }
        }
        free(__handler->r_max);
    }

    // Allocation
    if (__handler->r_allocation != NULL) {
        for (i = 0; i < p_count; i++) {
            if (__handler->r_allocation[i] != NULL) {
                free(__handler->r_allocation[i]);
            }
        }
        free(__handler->r_allocation);
    }

    // Need
    if (__handler->r_need != NULL) {
        for (i = 0; i < p_count; i++) {
            if (__handler->r_need[i] != NULL) {
                free(__handler->r_need[i]);
            }
        }
        free(__handler->r_need);
    }

    // Request
    if (__handler->r_request != NULL) {
        for (i = 0; i < p_count; i++) {
            if (__handler->r_request[i] != NULL) {
                free(__handler->r_request[i]);
            }
        }
        free(__handler->r_request);
    }

    // Free the handler itself
    free(__handler);
}