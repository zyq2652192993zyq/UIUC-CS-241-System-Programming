#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "format.h"

extern char **environ;

void split(char *str, char *delim, char **s)
{
	if (!str || !delim) return;
	
	char *ptr = strtok(str, delim);
	int cnt = 0;
	while (ptr) {
		s[cnt++] = ptr;
		ptr = strtok(NULL, delim);
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		char **ptr = environ;
		while (*ptr) {
			printf("%s\n", *ptr);
			++ptr;
		}

		return 0;
	}

	if (argc == 2) {
		print_env_usage();
	}

	//split the string
	int num = 0, n = strlen(argv[1]);
	for (int i = 0; i < n; ++i) {
		if (argv[1][i] == ',') ++num;
	}
	++num; //calculate the number of the variables

	char **s = malloc(num * sizeof(char *));
	split(argv[1], ",", s); 

	//edit the environment variable
	for (int i = 0; i < num; ++i) {
		int status = putenv(s[i]);
		if (status) print_environment_change_failed();
	}
	free(s);

	pid_t pid;
	if (pid = fork() < 0) print_fork_failed();
	if (pid == 0) {
		int status = execvp(argv[2], argv + 2); /* -1 on error */
		if (status) print_exec_failed();
	}
	else {
		waitpid(pid, NULL, 0);
	}

	return 0;
}