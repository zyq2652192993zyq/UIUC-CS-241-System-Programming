> # Mangled Mutexes

## Learning objectives

- Applying threads to an embarrassingly parallel problem.
- Become more familiar with synchronization primitives, namely mutexes.

## Overview

### Recommended Readings:

Before beginning this lab, make sure to read the following in the wikibook:

- [Pthreads, Part 1: Introduction](https://github.com/angrave/SystemProgramming/wiki/Pthreads%2C-Part-1%3A-Introduction)
- [Pthreads, Part 2: Usage in Practice](https://github.com/angrave/SystemProgramming/wiki/Pthreads%2C-Part-2%3A-Usage-in-Practice)
- [Synchronization Part 1: Mutex Locks](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-1%3A-Mutex-Locks)

### Problem Description:

Given two integers, `a` and `b`, you must compute the number of primes between these two integers inclusively (in the range `[a, b]`).

One approach to this problem is known the **trial division**. You can read more about it in this [useful tutorial](https://www.khanacademy.org/computing/computer-science/cryptography/comp-number-theory/a/trial-division).

## `count_primes.c`

We highly recommend starting the lab by attempting to write a version that works with just a single thread.

**When you have written a single threaded version**, we can start thinking about how to offload the work to multiple threads.

count_primes should be used as follows:

```bash
$ ./count_primes <start> <end> <number of threads>
```

Here the start is the first number in the interval you would like to check over, end is the last number. The number of threads is how many threads you should be creating via `pthread_create`.

So for example, if you wanted to compute the number of primes in the range [1,541] using 4 threads, this would be the expected output.

```bash
$ ./count_primes 1 541 4
There are 100 primes between 1 and 541 inclusive
```

**NOTE: Using 4 threads here means that you are to create 4 threads, so there will be 5 threads in your program when you include the main thread.**

There are a couple considerations you may want to make when writing your multithreaded code.

- How might you divide up the work for your threads?
- How might you ensure all the threads get a relatively even amount of work?
    - Does it take the same amount of time to determine if a number is prime for all numbers?
- How will you prevent data races (a race condition caused by concurrent reads and writes of a shared memory location)

As usual, your code should have no memory leaks.

## Testing your code

There is great resource known as `tsan ` that you can read more about [here](https://courses.engr.illinois.edu/cs241/sp2016/tsan). It is a tool from Google built into clang which can help you detect race conditions in your code.

------

这个`Lab`在[Pthreads, Part 2: Usage in Practice](https://github.com/angrave/SystemProgramming/wiki/Pthreads%2C-Part-2%3A-Usage-in-Practice)的Intro to Race Conditions部分提到。对于需要多个线程处理一段区间，最好的办法就是用一个结构体去分别记录每段区间的起始位置。这时候可以让所有线程去访问总的计数器，因为哪个线程先对计数器+1，还是后对计数器+1都不影响结果。

循环用`pthread_join`去销毁各个线程。这里我们默认线程的最大数目不会超过10。

```c
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

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
    printf("usage: ./count_primes <start> <end> <number of threads>\n");
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
```

```bash
$ make
./count_primes 1 541 4
```

