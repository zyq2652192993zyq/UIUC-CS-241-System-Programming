#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main()
{
    printf("NAME=%s\n", getenv("NAME"));

    char *ch = "NAME=PC";
    int status = putenv(ch);
    if (status) printf("error\n");
    printf("NAME=%s\n", getenv("NAME"));

    return 0;
}
