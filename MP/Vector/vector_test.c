/**
 * Machine Problem: Vector
 * CS 241 - Fall 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "vector.h"

// Test your vector here
void *copy_int(void *elem) {
	int *orig = (int *) elem;
	int *new  = malloc(sizeof(int));
	*new = *orig;
	return new;
}

void destory_int(void *elem) {
	free(elem);
}

int main() { 
	// Set up
	Vector *vector = Vector_create(copy_int, destory_int);
	int a = 10, b = 4;

	// TEST 1
	Vector_insert(vector, 8, (void *) &a);
	Vector_insert(vector, 2, (void *) &a);

	assert(*((int *) Vector_get(vector, 2)) == 10);
	assert(*((int *) Vector_get(vector, 9)) == 10);
	assert(Vector_size(vector) == 10);
	assert(Vector_capacity(vector) == 10);

	// TEST 2
	Vector_set(vector, 2, NULL);
	Vector_insert(vector, 2, (void *) &b);

	assert(*((int *) Vector_get(vector,  2)) ==  4);
	assert(*((int *) Vector_get(vector, 10)) == 10);
	assert(Vector_capacity(vector) == 20);


	// // TEST 3
	for (size_t iter = 0; iter < 8; iter++)
		Vector_delete(vector, 0);

	assert(*((int *) Vector_get(vector,  2)) == 10);
	assert(Vector_capacity(vector) == 10);


	// // TEST 4
	Vector_insert(vector, 70, (void *) &a);

	assert(Vector_capacity(vector) == 80);
	assert(Vector_size(vector)	   == 71);


	// // TEST 5
	Vector_set(vector, 69, (void *) &a);
	Vector_set(vector, 70, (void *) &b);

	assert(*((int *) Vector_get(vector, 69)) == 10);
	assert(*((int *) Vector_get(vector, 70)) ==  4);
	assert(Vector_capacity(vector) == 80);
	assert(Vector_size(vector)	   == 71);


	// // TEST 6
	Vector_append(vector, (void *) &a);
	assert(*((int *) Vector_get(vector, 71)) == 10);

	assert(Vector_capacity(vector) == 80);
	assert(Vector_size(vector)	   == 72);


	// // TEST 7
	Vector_insert(vector, 79, (void *) &a);

	assert(Vector_capacity(vector) == 80);
	assert(Vector_size(vector)	   == 80);

	Vector_append(vector, (void *) &b);

	assert(*((int *) Vector_get(vector,  2)) == 10);
	assert(*((int *) Vector_get(vector, 80)) ==  4);
	assert(Vector_capacity(vector) == 160);
	assert(Vector_size(vector)	   == 81);

	// // Clean up
	Vector_destroy(vector);
	vector = NULL;

	return 0;
 }

