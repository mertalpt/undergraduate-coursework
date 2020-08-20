/*
	CS342 - Project 1
	Author: Mert Alp Taytak
	ID: 21602061
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
	MACROS, DEFINITIONS and GLOBALS
*/

long M;

/*
	FUNCTION PROTOTYPES
*/

int process_m(const char *str);

int main(int argc, char **argv)
{
	if (argc != 2) {
		return -1;
	}

	process_m(argv[1]);

	char *buff = (char*)calloc(M, sizeof(char));
	for (long i = 0; i < M; i++) {
		buff[i] = 97 + (random() % 26); /* Random [a-z] */
		write(STDOUT_FILENO, &buff[i], 1);
	}

	return 0;
}

/*
	Reads argument for M
	Makes sure it is valid
*/
int process_m(const char* str)
{
	if (*str == '\0') {
		return -1;
	}

	char *endptr;
	long int res = strtol(str, &endptr, 10);

	if (*endptr == '\0') { /* Success */
		M = res;
		return 0;
	}
	return -1;
}