/*
	Creates, reads and writes files and loads the module to analyze the process
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// PROTOTYPES
void load_module();
void unload_module();

// GLOBALS

int main(void)
{
	const char *files[] = {
		"__tmp1.txt", "__tmp2.txt", "__tmp3.txt", "__tmp4.txt", "__tmp5.txt", 
		"__tmp6.txt", "__tmp7.txt", "__tmp8.txt", "__tmp9.txt", "__tmp10.txt"
	};
	int fds[10];
	char buff[64];

	// Print opening message
	printf("Starting the application.\n");

	// Load&Unload the module
	printf("Loading the module.\n");
	load_module(getpid());
	printf("Unloading the module.\n");
	unload_module();

	// Create files
	printf("Creating files.\n");
	for (int i = 0; i < 10; i++) {
		fds[i] = open(files[i], O_CREAT | O_WRONLY | O_SYNC);
		if (fds[i] < 0) {
			printf("Error at creating %s\n", files[i]);
			perror("Error");
			return -1;
		}
	}

	// Load&Unload the module
	printf("Loading the module.\n");
	load_module(getpid());
	printf("Unloading the module.\n");
	unload_module();

	// Write to files
	printf("Writing to files. May take a while.\n");
	for (int i = 0; i < 10; i++) {
		printf("Writing to %s\n", files[i]);
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 32; k++) {
				write(fds[i], "0000000000000000000000000000000000000000000000000000000000000000", 64);
			}
		}
	}

	// Load&Unload the module
	printf("Loading the module.\n");
	load_module(getpid());
	printf("Unloading the module.\n");
	unload_module();

	// Close files
	printf("Closing the files.\n");
	for (int i = 0; i < 10; i++) {
		close(fds[i]);
	}

	// Reopen files
	printf("Reopening files.\n");
	for (int i = 0; i < 10; i++) {
		fds[i] = open(files[i], O_RDONLY);
		if (fds[i] < 0) {
			printf("Error at reopening %s\n", files[i]);
			perror("Error");
			return -1;
		}
	}

	// Read files
	printf("Reading files. May take a while.\n");
	for (int i = 0; i < 10; i++) {
		printf("Reading %s\n", files[i]);
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 32; k++) {
				read(fds[i], buff, 64);
			}
		}
	}

	// Load&Unload the module
	printf("Loading the module.\n");
	load_module(getpid());
	printf("Unloading the module.\n");
	unload_module();

	// Close files
	printf("Closing the files.\n");
	for (int i = 0; i < 10; i++) {
		close(fds[i]);
	}

	// Cleanup and return
	printf("Removing files.\n");
	for (int i = 0; i < 10; i++) {
		remove(files[i]);
	}

	return 0;
}

void load_module(pid_t pid)
{
	char buff[256];
	sprintf(buff, "insmod print_module.ko processpid=%d", pid);
	system(buff);
}

void unload_module()
{
	system("rmmod print_module");
}