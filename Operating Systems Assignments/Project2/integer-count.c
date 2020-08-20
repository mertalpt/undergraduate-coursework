#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "hash.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Macros and Definitions

#define BUFF_SIZE 8

// Linked List Implementation For Sorted Linked List
//(Influenced by GeekforGeeks (https://www.geeksforgeeks.org/given-a-linked-list-which-is-sorted-how-will-you-insert-in-sorted-way/))
struct Node {
	struct Node *next;
	int unique_key;
};

void sortedInsert(struct Node** head_ref, struct Node* new_node)
{
    struct Node* current;
    /* Special case for the head end */
    if (*head_ref == NULL || (*head_ref)->unique_key >= new_node->unique_key)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = *head_ref;
        while (current->next!=NULL && current->next->unique_key < new_node->unique_key)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

struct Node *newNode(int new_data)
{
    /* allocate node */
    struct Node* new_node =
        (struct Node*) malloc(sizeof(struct Node));

    /* put in the data  */
    new_node->unique_key  = new_data;
    new_node->next =  NULL;

    return new_node;
}

void writeToOutputFile(struct Node *head, HashTable *hp, char *output_file_name)
{
    struct Node *temp = head;
    void *item;
    FILE *fd;

    fd = fopen(output_file_name, "w");

    while(temp != NULL)
    {
        if (!hash_get(hp, temp->unique_key, &item)) {
            fprintf(fd, "%d: %d\n", temp->unique_key, (int) item);
		}

        temp = temp->next;
    }

    fclose(fd);
}

// Globals

const int TABLE_SIZE = 1000;
const int LOCK_COUNT = 100;
HashTable *hp;
pthread_t *threads;
sem_t mutex;
struct Node* head = NULL;

// Function Prototypes

void *process_file(void *input);
int process_line(FILE *stream, int *num);

// Functions

int main(int argc, char **argv)
{
	if(sem_init(&mutex, 0, 1) < 0)
	{
		perror("Semaphore initilization failed.\n");
		exit(0);
	}

	if (argc < 2) {
		fprintf(stderr, "Invalid number of arguments.\n");
		return -1;
	}

	char *endptr;
	long int num_inputs = strtol(argv[1], &endptr, 10);

	if (*endptr != '\0') {
		fprintf(stderr, "Invalid argument for number of inputs.\n");
		return -1;
	}

	if (num_inputs + 3 != argc) {
		fprintf(stderr, "Invalid number of arguments.\n");
		return -1;
	}

	// Initialize hash table
	hp = hash_init(TABLE_SIZE, LOCK_COUNT);

	// Create threads
	threads = calloc(num_inputs, sizeof(pthread_t));

	for (int i = 0; i < num_inputs; i++) {
		pthread_create(&threads[i], NULL, process_file, (void*)argv[i+2]);
	}

	// Run threads
	for (int i = 0; i < num_inputs; i++) {
		pthread_join(threads[i], NULL);
	}

	// Generate the output file and write into it
	writeToOutputFile(head, hp, argv[num_inputs + 2]);

	// Cleanup and return

	// Free hash table
	hash_destroy(hp);

	// Free thread array
	free(threads);

	// Free key list
	struct Node *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return 0;
}

void *process_file(void *input)
{
	// Open input file
	const char *file_name = (char*)input;
	FILE *fp = fopen(file_name, "r");
	struct Node *new_node;// = newNode(5);

	if (!fp) {
		fprintf(stderr, "Could not open file.\n");
		return NULL;
	}

	// Process file
	int curr_num;
	void *item;

	while (process_line(fp, &curr_num) == 0) {
		sem_wait(&mutex);
		// Update if value exists
		if (!hash_get(hp, curr_num, &item)) {
			hash_update(hp, curr_num, (void*)((int)item)+ 1);
		}
		// Create record otherwise
		else {
			hash_insert(hp, curr_num, (void*)1);
			new_node = newNode(curr_num);
			sortedInsert(&head, new_node);
		}
		sem_post(&mutex);
	}

	fclose(fp);
	return NULL;
}

/*
	Reads a line from the stream
	Tokenizes the line by whitespace
	Returns the tokens in a string array
*/
int process_line(FILE *stream, int *num)
{
	// Read line
	int buff_size = BUFF_SIZE;
	char *buff = (char*)calloc(buff_size, sizeof(char));

	char last_char = fgetc(stream);
	int curr = 0;
	int flag = 0;

	// Check if input is EOF
	if (last_char == EOF) {
		free(buff);
		return -1;
	}
	else {
		buff[curr++] = last_char;
	}

	// Read rest of the input
	do {
		for (; curr < buff_size; curr++) {
			last_char = fgetc(stream);
			if (last_char == '\n' || last_char == EOF) {
				flag = !flag;
				break;
			}
			buff[curr] = last_char;
		}

		// Extend buffer
		buff_size += BUFF_SIZE;
		char *temp = (char*)calloc(buff_size, sizeof(char));
		memcpy(temp, buff, curr);
		free(buff);
		buff = temp;
	} while (!flag);

	// Convert to integer
	char *endptr;
	long int res = strtol(buff, &endptr, 10);

	if (*endptr != '\0') {
		fprintf(stderr, "File input not a number.\n");
		free(buff);
		return -1;
	}

	*num = (int)res;

	// Cleanup and return
	free(buff);
	return 0;
}