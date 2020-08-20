/*
	CS342 - Project 1
	Author: Mert Alp Taytak
	ID: 21602061
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
	MACROS, DEFINITIONS and GLOBALS
*/

#define BUFF_SIZE 64
#define ARG_COUNT 8

#define ERR_MSG(msg) fprintf(stderr, "Error! %s\n", msg)

int N;
const char *shell_prompt = "[bilshell]>";

/*
	FUNCTION PROTOTYPES
*/

int read_line(FILE *stream, int *argc, char ***buff_ptr);
int process_n(const char *str);
int process_command(int argc, char **args);
int interactive_mode(char **args);
int batch_mode(char **args);

/*
	FUNCTION DEFINITIONS
*/

int main(int argc, char **argv)
{
	switch (argc) {
		case 2: /* Interactive mode */
			return interactive_mode(argv);
			break;
		case 3: /* Batch mode */
			return batch_mode(argv);
			break;
		default:
		fprintf(stderr, "Error! Invalid number of arguments received.\n");
	}
	return 0;
}

/*
	Reads a line from the stream
	Tokenizes the line by whitespace
	Returns the tokens in a string array
*/
int read_line(FILE *stream, int *argc, char ***buff_ptr)
{
	// Read line
	int buff_size = BUFF_SIZE;
	char *buff = (char*)calloc(buff_size, sizeof(char));

	char last_char = fgetc(stream);
	int curr = 0;
	int flag = 0;

	// Check if input is EOF
	if (last_char == EOF) {
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
	
	// Split by whitespace
	const char *delim = " \t";
	int arg_size = ARG_COUNT;
	char **args = (char**)calloc(arg_size, sizeof(char*));
	
	char *token = strtok(buff, delim);
	if (token == NULL) { /* Invalid line */
		free(buff);
		free(args);
		ERR_MSG("Input line is all whitespace.");
		return -1;
	}
	else {
		args[0] = token;
	}

	curr = 1;
	flag = 0;

	do {
		for (; curr < arg_size; curr++) {
			token = strtok(NULL, delim);
			if (token == NULL) {
				flag = !flag;
				break;
			}
			args[curr] = token;
		}

		// End of buffer
		if (flag) {
			break;
		}

		// Extend arguments array
		arg_size += ARG_COUNT;
		char **temp = (char**)calloc(arg_size, sizeof(char*));
		for (int i = 0; i < curr; i++) {
			temp[i] = args[i];
		}
		free(args);
		args = temp;
	} while (!flag);

	// Cleanup and return
	char **temp = (char**)calloc((curr+1), sizeof(char*));
	int tok_size;
	for (int i = 0; i < curr; i++) {
		token = args[i];
		tok_size = strlen(token);
		temp[i] = (char*)calloc(tok_size, sizeof(char));
		memcpy(temp[i], token, tok_size);
	}
	// Null terminated string array for exec syscall
	temp[curr] = NULL;

	free(buff);
	free(args);
	*argc = curr;
	*buff_ptr = temp;
	return 0;
}

/*
	Reads argument for N
	Makes sure it is valid
*/
int process_n(const char* str)
{
	if (*str == '\0') {
		ERR_MSG("Invalid N input.");
		return -1;
	}

	char *endptr;
	long int res = strtol(str, &endptr, 10);

	if (*endptr == '\0') { /* Success */
		N = (int) res;
		// Range check
		if (0 < N && N < 4097) { return 0; }
		else { ERR_MSG("N value out of range."); return -1; }
	}
	return -1;
}

/*
	Takes argument array and executes the given command
*/
int process_command(int argc, char **args)
{
	// Reject commands ending with '|'
	if (!strcmp("|", args[argc-1])) {
		ERR_MSG("Second command in composite command is empty.");
		return -1;
	}

	// Determine if single command or composite command
	int comp_c = 0;
	int pipe_ind;
	for (int i = 0; i < argc; i++) {
		if (!strcmp("|", args[i])) {
			comp_c++;

			// Split argument array for execv
			if (comp_c == 1) {
				pipe_ind = i;
				char *pipe_sym = args[i];
				args[i] = NULL;
				free(pipe_sym);
			}
		}
	}

	// Locals
	int stat;
	pid_t p_id1;
	pid_t p_id2;
	int fds1[2];
	int fds2[2];
	long char_c = 0;
	long read_call_c = 0;

	switch (comp_c) {
		case 0: /* Single command */
			p_id1 = fork();

			if (p_id1 > 0) { /* Parent */
				wait(&p_id1);
				return 0;
			}
			else if (p_id1 == 0) { /* Child */
				return execvp(args[0], args);
			}
			else { /* Error */
				ERR_MSG("Could not create a child process.");
				return -1;
			}
			break;
		case 1: /* Two commands */
			// Create the pipes
			stat = pipe(fds1);
			if (stat < 0) { ERR_MSG("Could not create first pipe."); return -1; }
			stat = pipe(fds2);
			if (stat < 0) { ERR_MSG("Could not create second pipe."); return -1; }

			// Create the children
			p_id1 = fork();
			if (p_id1 > 0) { /* Parent */
				// Create the second child
				p_id2 = fork();
				if (p_id2 > 0) { /* Parent */
					// Close unused ends of the pipes
					stat = close(fds1[1]);
					if (stat < 0) { ERR_MSG("Could not close write end of first pipe."); return -1; }
					stat = close(fds2[0]);
					if (stat < 0) { ERR_MSG("Could not close read end of second pipe."); return -1; }

					// Do the RW
					char *buff = (char*)calloc(N, sizeof(char));
					while ((stat = read(fds1[0], buff, N)) > 0) {
						char_c += stat;
						read_call_c++;

						stat = write(fds2[1], buff, stat);						
						if (stat < 0) { ERR_MSG("Could not write to pipe."); return -1; }
						
						memset(buff, '\0', N); /* Reset buffer */
					}

					// Print stats after execution finishes
					close(fds2[1]);
					close(fds1[0]);
					wait(&p_id1);
					wait(&p_id2);
					fprintf(stdout, "character-count: %ld\n", char_c);
					fprintf(stdout, "read-call-count: %ld\n", read_call_c);
					
					// Cleanup and return
					free(buff);
					return 0;
				}
				else if (p_id2 == 0) { /* Second Child */
					// Not needed
					close(fds1[0]);
					close(fds1[1]);
					close(fds2[1]);

					dup2(fds2[0], STDIN_FILENO); /* Redirect standard input */
					return execvp(args[pipe_ind+1], args + (pipe_ind+1));
				}
				else { /* Error */
					ERR_MSG("Could not create second child process.");
					return -1;
				}
			}
			else if (p_id1 == 0) { /* First Child */
				// Not needed
				close(fds2[0]);
				close(fds2[1]);
				close(fds1[0]);

				dup2(fds1[1], STDOUT_FILENO); /* Redirect standard output */
				return execvp(args[0], args);
			}
			else { /* Error */
				ERR_MSG("Could not create first child process.");
				return -1;
			}
			break;
		default: /* Invalid input */
		ERR_MSG("Invalid command.");
		return -1;
	}
}

int interactive_mode(char **args)
{
	int stat; /* To store status messages */
	stat = process_n(args[1]);
	if (stat < 0) {
		ERR_MSG("Invalid N input.");
		return -1;
	}

	// REPL
	int argc;
	char **buff;
	while(1) {
		fprintf(stdout, "%s ", shell_prompt);
		stat = read_line(stdin, &argc, &buff);
		if (stat < 0) {
			ERR_MSG("Could not read from shell.");
			return -1;
		}

		// Check for exit
		const char *exit_key = "exit";
		if (!strcmp(buff[0], exit_key)) {
			return 0;
		}

		// Otherwise, execute command
		stat = process_command(argc, buff);

		// Check for errors
		if (stat < 0) {
			ERR_MSG("Could not process command.");
			return -1;
		}
	}

	return 0;
}

int batch_mode(char **args)
{
	int stat; /* To store status messages */
	stat = process_n(args[1]);
	if (stat < 0) {
		ERR_MSG("Invalid N input.");
		return -1;
	}

	// Open the file
	FILE *fp = fopen(args[2], "r");
	if (!fp) { ERR_MSG("Could not open file."); return -1; }

	char **buff;
	int argc;

	// REPL for file
	while (!read_line(fp, &argc, &buff)) {
		// Print current command
		fprintf(stdout, "%s", shell_prompt);
		for (int i = 0; i < argc; i++) {
			fprintf(stdout, " %s", buff[i]);
		}
		fprintf(stdout, "\n");

		// Process current command
		stat = process_command(argc, buff);

		// Free buffer
		for (int i = 0; i < argc; i++) {
			free(buff[i]);
		}
		free(buff);

		// Check for errors
		if (stat < 0) {
			fclose(fp);
			ERR_MSG("Could not read from file.");
			return -1;
		}
	}

	fclose(fp);
	return 0;
}