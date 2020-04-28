#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

/*
 * Vector structure
 * Do not modify the structure
 * array: Void pointer to the beginning of an array of void pointers to
 * arbitrary data.
 * size: The number of elements in the vector. This is the number of actual
 * objects held in the vector, which is not necessarily equal to its capacity.
 * capacity: The size of the storage space currently allocated for the vector,
 * expressed in terms of elements.
 * copyt_constructor: the function callback for you to define the way you want
 * to copy elements
 * destructor:  the function callback for you to define the way you want to
 * destroy elements
 */
struct Vector {
    copy_constructor_type copy_constructor;
    destructor_type destructor;

    void **array;
    size_t size;
    size_t capacity;
};

// HELPER FUNCTION - UNSAFE, CALLER MUST PERFORM ALL CHECKS
void Vector_dealloc_elem(Vector *vector, size_t index) {
    void *elem;
    if ((elem = vector -> array[index]))
        (*vector -> destructor)(elem);
}

// COMPLETE construct function
Vector *Vector_create(copy_constructor_type copy_constructor, destructor_type destructor) {
    Vector *vector = malloc(sizeof(Vector));
    assert(vector); /* check whether the malloc success or not */

    vector -> copy_constructor = copy_constructor;
    vector -> destructor = destructor;
    vector -> array = malloc(INITIAL_CAPACITY * sizeof(void *));
    /* Initialize the array element with NULL pointer*/
    for (size_t i = 0; i < INITIAL_CAPACITY; ++i)
        vector -> array[i] = NULL;

    vector -> size = 0;
    vector -> capacity = INITIAL_CAPACITY;
    return vector;
}

// COMEPLETE destruct function
void Vector_destroy(Vector *vector) {
	assert(vector);

	for (size_t i = 0; i < vector -> capacity; ++i) {
		Vector_dealloc_elem(vector, i);
	}

	free(vector -> array);
	free(vector);
}

// COMPLETE
size_t Vector_size(Vector *vector) {
    assert(vector);
    return vector -> size;
}

// COMPLETE
size_t Vector_capacity(Vector *vector) {
  	assert(vector);
  	return vector -> capacity;
}

// COMPLETE
void Vector_resize(Vector *vector, size_t new_size) {
    assert(vector);
    if (new_size == Vector_size(vector)) return;

    size_t new_capacity = vector -> capacity;

    if (new_size > Vector_capacity(vector)) {
        while (new_capacity < new_size)
            new_capacity *= 2;

        vector -> array = realloc(vector -> array, new_capacity * sizeof(void *));
        assert(vector -> array);

        // initialize the new element with NULL
        for (size_t i = Vector_size(vector); i < new_capacity; ++i)
            vector -> array[i] = NULL;
    }
    else if (vector -> capacity > INITIAL_CAPACITY && vector -> capacity > 2 * new_size) {
        while (new_capacity > 2 * new_size) new_capacity >>= 1;
        if (new_capacity < INITIAL_CAPACITY) new_capacity = INITIAL_CAPACITY;

        for (int i = new_size; i < new_capacity; ++i) {
            Vector_dealloc_elem(vector, i);
        }

        vector -> array = realloc(vector -> array, new_capacity * sizeof(void *));
        assert(vector -> array);
    }
    else { //capacity is enough
        if (new_size > vector -> size) {
            for (size_t i = vector -> size; i < new_size; ++i)
                vector -> array[i] = NULL;
        }
        else {
            for (size_t i = new_size; i < vector -> size; ++i)
                Vector_dealloc_elem(vector, i);
        }
    }

    vector -> size = new_size;
    vector -> capacity = new_capacity;
}


// COMPLETE
void Vector_set(Vector *vector, size_t index, void *elem) {
    assert(vector);
    assert(index < vector -> size);

    void *old;
    if (old = vector -> array[index]) {
    	(*vector -> destructor)(old);
    }

    vector -> array[index] = elem ? (*vector -> copy_constructor)(elem) : NULL;
}

// COMPLETE
void *Vector_get(Vector *vector, size_t index) {
    assert(vector);
    assert(index < vector -> size);

    return vector -> array[index];
}

// COMPLETE
void Vector_insert(Vector *vector, size_t index, void *elem) {
    assert(vector);

    if (index < vector -> size) {
        Vector_resize(vector, vector -> size + 1);
        for (size_t i = vector -> size - 1; i > index; --i)
            vector -> array[i] = vector -> array[i - 1];
        vector -> array[index] = elem ? (*vector -> copy_constructor)(elem) : NULL;
    }
    else {
        Vector_resize(vector, index + 1);
        vector -> array[index] = elem ? (*vector -> copy_constructor)(elem) : NULL;
    }
}

// COMPLETE
void Vector_delete(Vector *vector, size_t index) {
    assert(vector);
    assert(index < vector -> size);

    Vector_dealloc_elem(vector, index);
    for (size_t i = index; i < vector -> size - 1; ++i)
        vector -> array[i] = vector -> array[i + 1];

    vector -> array[vector -> size - 1] = NULL;
    Vector_resize(vector, vector -> size - 1);
}   

// COMPLETE
void Vector_append(Vector *vector, void *elem) {
    assert(vector);

    Vector_resize(vector, vector -> size + 1);
    vector -> array[vector -> size - 1] = elem ? (*vector -> copy_constructor)(elem) : NULL;
}
