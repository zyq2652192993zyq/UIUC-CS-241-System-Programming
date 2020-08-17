#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int cnt = 0;
const int maxThreadNum = 10;

struct Node
{
    pthread_t tid;
    int start, end;
};

bool isPrime(int num)
{
    if (num <= 1) return false;
    if (num == 2) return true;
    if (!(num & 1)) return false;

    int limit = (int)sqrt((double)num) + 1;
    for (int i = 3; i <= limit; i += 2) {
        if (num % i == 0) return false;
    }
       
    return true;
}

void *countPrime(void * param)
{
    struct Node *node = (struct Node *)param;

    for (int i = node -> start; i <= node -> end; ++i) {
        if (isPrime(i)) ++cnt;
    }

    return NULL;
}

void usage()
{
    printf("usage: ./main <start> <end> <number of threads>\n");
    exit(1);
}


int main(int argv, char *argc [])
{
    if (argv < 4) usage();
    
    struct Node Info[maxThreadNum];

    int start = atoi(argc[1]), end = atoi(argc[2]);
    int threadNum = atoi(argc[3]);

    int len = (end - start) / threadNum;

    int last = start;
    for (int i = 0; i < threadNum; ++i) {
        Info[i].start = last;

        if (i == threadNum - 1) Info[i].end = end;
        else Info[i].end = last + len;

        last = Info[i].end + 1;
    }

    for (int i = 0; i < threadNum; ++i) {
        pthread_create(&Info[i].tid, NULL, countPrime, &Info[i]);
    }

    for (int i = 0; i < threadNum; ++i) {
        pthread_join(Info[i].tid, NULL);
    }

    printf("There are %d primes between %d and %d inclusive\n", cnt, start, end);

    return 0;
}
