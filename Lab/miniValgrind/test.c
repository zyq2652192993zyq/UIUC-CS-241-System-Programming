#include "mini_valgrind.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SMALL_SIZE 4
#define NORMAL_SIZE 8
#define LARGE_SIZE 16

int main()
{
	void *p;
	//TEST 1 - test malloc and free
	p = malloc(NORMAL_SIZE);
	free(p);

	//TEST 2 - test malloc and realloc
	p = malloc(NORMAL_SIZE);
	p = realloc(p, LARGE_SIZE);
	free(p);

	//TEST 3 - test realloc
	p = malloc(LARGE_SIZE);
	p = realloc(p, SMALL_SIZE);
	free(p);

	//TEST 4 - test realloc
	p = malloc(NORMAL_SIZE);
	p = realloc(p, NORMAL_SIZE);
	free(p);

	//TEST 5
	p = malloc(NORMAL_SIZE);
	strcpy((char *)p, "abcdefg");
	assert(*(char *)p == 'a');

	p = realloc(p, SMALL_SIZE);
	assert(*(char *)p == 'a');

	strcpy((char *)p, "def");
	assert(*(char *)p == 'd');

	p = realloc(p, LARGE_SIZE);
	assert(*(char *)p == 'd');

	strcpy((char *)p, "0123456789abcde");
	assert(*(char *)p == '0');

	p = realloc(p, SMALL_SIZE);
	assert(*(char *)p == '0');
	free(p);

	//TEST 6 - bad free
	free(p);

	//TEST 7 - test free NULL pointer
	p = NULL;
	free(p);

	//print report
	print_report();

	return 0;
}