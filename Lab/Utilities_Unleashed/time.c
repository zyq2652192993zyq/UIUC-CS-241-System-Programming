#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> /* clock_gettime() declare here */
#include "format.h"

int main(int argc, char *argv[])
{
    //usage error
    if (argc < 2) print_time_usage();

    char **command = malloc(argc * sizeof(char*));
    for (int i = 0; i < argc - 1; ++i) { //remove ./time
        command[i] = argv[i + 1];
    }
    command[argc - 1] = '\0';

    pid_t pid = fork();
    if (pid < 0) {
        free(command);
        print_fork_failed(); //fork error
    }
    else if (pid == 0) {
        int status = execvp(command[0], command); /* -1 on error */
        if (status) print_exec_failed();
    }
    else {
        //record start time and end time
        struct timespec start, end;
        int status; 

        clock_gettime(CLOCK_MONOTONIC, &start);
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        //calculate time in seconds
        double duration = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        display_results(argv, duration);
    }   
    free(command);

    return 0;
}