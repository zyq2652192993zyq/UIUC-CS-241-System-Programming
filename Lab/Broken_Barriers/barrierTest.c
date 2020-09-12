#include "barrier.h"
#include "algorithm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
	char *buffer;
	int len;
	barrier_t barrier1, barrier2;
} Foo;

Foo *create()
{
	Foo *obj = (Foo*) malloc(sizeof(Foo));

	obj -> buffer = malloc(17 * sizeof(char));
	obj -> len    = 0;

	barrier_init(&obj -> barrier1, 2);
	barrier_init(&obj -> barrier2, 2);

	return obj;
}

void printFirst(Foo *obj)
{
	obj -> len += sprintf(obj -> buffer + obj -> len, "First");
}

void printSecond(Foo *obj)
{
	obj -> len += sprintf(obj -> buffer + obj -> len, "Second");
}

void printThird(Foo *obj)
{
	obj -> len += sprintf(obj -> buffer + obj -> len, "Third");
}

void* first(void *para)
{
	Foo *obj = (Foo *)para;

	printFirst(obj); 

	barrier_wait(&obj -> barrier1);

	return (void *)NULL;
}

void* second(void *para)
{
	Foo *obj = (Foo *)para;

	barrier_wait(&obj -> barrier1);

	printSecond(obj);

	barrier_wait(&obj -> barrier2);

	return (void *)NULL;
}

void* third(void *para)
{
	Foo *obj = (Foo *)para;

	barrier_wait(&obj -> barrier2);

	printThird(obj);

	return (void *)NULL;
}

void destroy(Foo *obj)
{
	barrier_destroy(&obj -> barrier1);
	barrier_destroy(&obj -> barrier2);

	free(obj -> buffer);
	free(obj);
}



int main()
{
	char *res = "FirstSecondThird";
	int seq [] = {1, 2, 3};
	int cnt    = 0;

	do {
	    ++cnt;
        Foo *obj = create();

		pthread_t tid[3];
		for (int i = 0; i < 3; ++i) {
			switch (seq[i]) {
				case 1: pthread_create(&tid[i], NULL, first, (void *)obj); break;
				case 2: pthread_create(&tid[i], NULL, second, (void *)obj); break;
				default: pthread_create(&tid[i], NULL, third, (void *)obj);
			}
		}

		for (int i = 0; i < 3; ++i) {
			pthread_join(tid[i], NULL);
		}

		assert(strcmp(res, obj -> buffer) == 0);

		destroy(obj);
	} while (next_permutation(seq, 0, 3));

	printf("%d %s\n", cnt, "tests passed!");

	return 0;
}