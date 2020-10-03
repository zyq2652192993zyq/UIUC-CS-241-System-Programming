#define _GNU_SOURCE

#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * This function will parse your program arguments for you.

 * When it is done it will return an array of void pointers.
 * Suppose that void ** args = arg_parse(int argc, char *argv[]):
 *  args[0] - is a pointer to the number of lines in the file
 *  args[1] - is a pointer to the number of bytes in the longest line
 *  args[2] - is a FILE pointer to the input file
 *  args[3] - is a FILE pointer to the output file
 *
 * Features:
 *  Will prompt the user if they don't have the right arguments
 *  Will prompt the user if the input file is not readable
 *  Will prompt the user if the output file is not writable
 *  Will rewind() the FILE pointer for the input file
 *
 * Note: When you are done using the args returned
 *  Call free_args to clean up memory and close files
 */

void **arg_parse(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    // Testing that the input and output files are valid ...
    char *input_file_name = argv[1];
    FILE *input_file      = fopen(input_file_name, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Was not able to read from the input file. Exiting...\n");
        exit(1);
    }

    char *output_file_name = argv[2];
    FILE *output_file      = fopen(output_file_name, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Was not able to write to the output file. Exiting...\n");
        exit(1);
    }

    // Open input file
    //  -> figure out how many lines it has and how long the longest line is
    char *line              = NULL;
    size_t len              = 0;
    ssize_t bytes_read;
    size_t *num_lines       = calloc(1, sizeof(size_t));
    size_t *max_line_length = calloc(1, sizeof(size_t));

    while ((bytes_read = getline(&line, &len, input_file)) != -1) {
        // Figuring out how many lines the file has
        (*num_lines)++;
        // Updating the max line length
        *max_line_length = *max_line_length > (size_t)bytes_read
                           ? *max_line_length
                           : (size_t)bytes_read;
    }
    free(line);
    rewind(input_file); // move pointer back to begining of the file

    // Ok return all this information to the user

    // One void pointer for each of the return values
    void **retvals = malloc(4 * sizeof(void *));
    retvals[0]     = num_lines;
    retvals[1]     = max_line_length;
    retvals[2]     = input_file;
    retvals[3]     = output_file;

    return retvals;
}

// Call this function to free the args returned from arg_parse()
void free_args(void **args) {
    free(args[0]);   // Freeing size_t *num_lines
    free(args[1]);   // Freeing size_t *max_line_length
    fclose(args[2]); // Closing the input file
    fclose(args[3]); // Closing the output file
    free(args);      // Freeing the args array
}

int main(int argc, char *argv[]) 
{
    void **args  = arg_parse(argc, argv);
    pid_t parentId = getpid();

    int lineNum = *(int*)args[0];
    int maxSize  = *(int*) args[1];
    FILE * inputfile  = args[2];
    FILE * outputfile = args[3];

    int pipes[lineNum][2];
    char *line  = NULL;
    char *lines[lineNum];
    int index       = 0;
    size_t size = 0;

    while(getline(&line, &size, inputfile) > 0) {
        lines[index] = strdup(line);
        ++index;
    }
    free(line);
    pid_t children[lineNum];

    for(int i = 0; i < lineNum; ++i) {
        children[i] = -2;
        pipe(pipes[i]);
        fcntl(pipes[i][1], F_SETPIPE_SZ, maxSize);
    }  

    for(int i = 0; i < lineNum; ++i) {
        if(getpid() == parentId) children[i] = fork();
        else break;
    }

    if(getpid() == parentId) {
        char buffer[4096];
        for(int i = 0; i < lineNum; ++i) {
            close(pipes[i][1]);
            waitpid(children[i], NULL, 0);
            read(pipes[i][0], buffer, 4096);
            close(pipes[i][0]);
            fprintf(outputfile, "%s", buffer);
        }
    }
    else {
        for(int i = 0; i < lineNum; ++i) {
            if(children[i] == 0) {
                close(pipes[i][0]);
                char * decrypted = get_most_likely_print_out(lines[i]); 
                write(pipes[i][1], decrypted, strlen(decrypted)+1);
                close(pipes[i][1]);
                free(decrypted);
                break;
            }
        }
    }
    free_args(args);
    for(int i = 0; i < lineNum; ++i) free(lines[i]);

    return 0;
}