#ifndef _vector_h
#define _vector_h

#include <stdio.h>

struct Vector;
typedef struct Vector Vector;

/*
 * This callback function takes in a pointer to arbitary
 * and returns a void pointer pointing to a copy of said data.
*/
typedef void *(*copy_constructor_type)(void *elem);

/*
 * This callback function takes in a pointer to arbitary data
 * and frees the memory allocated for it.
*/
typedef void (*destructor_type)(void *elem);

/* Allocate and return a new Vector structure. */
Vector *Vector_create(copy_constructor_type c, destructor_type d);

/**
 * Deallocate a Vector structure.
 * Every non-NULL entry in array from array[0] to array[size-1] will
 * need to be 'destroyed'.
*/
void Vector_destroy(Vector *vector);

/* Return the number of entries in use in this array. */
size_t Vector_size(Vector *vector);

/* Return the capacity of the vector. */
size_t Vector_capacity(Vector *vector);

/**
 * Sets the size of the array.
 *
 * If the array grows, new entries will be initialized to NULL.
 * If the array shrinks, entries past the new end of the array
 * will be deallocated.
 *
 * If the 'new_size' is larger than the capacity of the array, the
 * capacity will need to be increased, meaning the array will need to
 * be reallocated. You should keep doubling the capacity until it is bigger
 * than the 'new_size'.
 *
 * Also, if the new size of vector is less than or equal to 1/4th the capacity,
 * then you should keep halving the capacity until it is just about twice the
 * 'new size'.
 * However, to avoid repeated realloc()s with small capacity, maintain a minimum
 * capacity of 10.
 *
*/
void Vector_resize(Vector *vector, size_t new_size);

/**
 * Allocate a copy of 'elem' and store it in array[index].  If 'elem' is NULL,
 * just store NULL in array[index]. Note that if the vector is allocating
 * memory for the entry (which will happen when the entry is not NULL), then it
 * should also be the freeing it (hint,hint).
*/
void Vector_set(Vector *vector, size_t index, void *elem);

/**
 * Returns the entry stored at array[index].
 * If array[index] == NULL, this will return NULL.
 *
 * This does not return a copy of the entry; it returns the actual
 * void* stored at array[index]. This means the caller must not
 * deallocate the entry (if the entry is non-null).
*/
void *Vector_get(Vector *vector, size_t index);

/**
 * Inserts a copy of 'elem' or NULL if elem is NULL then stores it in
 * array[index].
 *
 * Note the array[index] could already contain a entry, in which case the entry
 * at array[index] and all subsequent entries should be shifted down.
 *
 * Also note that index could be well beyond the position of the last entry. In
 * which case the vector should resize() to accommodate that new entry.
*/
void Vector_insert(Vector *vector, size_t index, void *elem);

/**
 * Removes whatever entry is contained in array[index].
 *
 * Note that index does not have to be the last element of the vector, in which
 * case you should not only remove the entry at array[index], but also shift all
 * subsequent entries down.
 *
 * Do not forget to resize the vector accordingly.
*/
void Vector_delete(Vector *vector, size_t index);

/**
 * Appends a copy of 'elem' (or NULL) right after the last element in the
 * vector.
*/
void Vector_append(Vector *vector, void *elem);

#endif