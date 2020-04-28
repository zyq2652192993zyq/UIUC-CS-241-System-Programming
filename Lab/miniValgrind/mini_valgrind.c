#include "mini_valgrind.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  Replace the system malloc, realloc and free
 *  the system malloc will create meta data info
 */
#undef malloc
#undef realloc
#undef free

/**
 * Replace normal malloc, this malloc will also create meta data info
 * and insert it to the head of the list.
 * You have to malloc enough size to hold both the size of your allocated
 * and the meta_data structure.
 * In this function, you should only call malloc only once.
 *
 * @param size
 *  Size of the memory block, in bytes.
 * @param file
 *  Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *  Line in the file that causes memory leak
 *
 * @return
 *  On success, return a pointer to the memory block allocated by
 *  the function. Note that this pointer should return the actual
 *  address the pointer starts and not the meta_data.
 *
 *  If the function fails to allocate the requested block of memory,
 *  return a null pointer.
 */

void *mini_malloc(size_t size, const char *file, size_t line)
{
	void *memory = malloc(sizeof(meta_data) + size);
	if (!memory) return NULL;

	meta_data* md = (meta_data*)memory;
	insert_meta_data(md, size, file, line);
	total_usage += size;

	return memory + sizeof(meta_data);
}

/*
 * Replace normal realloc, this realloc will also first check whether the
 * pointer that passed in has memory. If it has memory then resize the memory
 * to it. Or if the pointer doesn't have any memory, then call malloc to
 * provide memory.
 * For total usage calculation, if the new size is larger than the old size,
 * the total usage should increase the difference between the old size and the
 * new size. If the new size is smaller or equal to the old size, the total
 * usage should remain the same.
 * You have to realloc enough size to hold both the size of your allocated
 * and the meta_data structure.
 * In this function, you should only call malloc only once.
 *
 * @param ptr
 *  The pointer require realloc
 * @param size
 *  Size of the memory block, in bytes.
 * @param file
 *  Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *  Line in the file that causes memory leak
 *
 * @return
 *  On success, return a pointer to the memory block allocated by
 *  the function. Note that this pointer should return the actual
 *  address the pointer starts and not the meta_data.
 *
 *  If the function fails to allocate the requested block of memory,
 *  return a null pointer.
 */
void *mini_realloc(void *ptr, size_t size, const char *file, size_t line)
{
	//check if the ptr passed has memory
	if (!ptr) mini_malloc(size, file, line);

	meta_data *pre = NULL, *cur = head, *next = NULL;
	while (cur) {
		next = cur -> next;
		void *tmp = (void*)cur + sizeof(meta_data);

		//if we find the memory block
		if (tmp == ptr) {
			//if the size has no change, just update the info
			if (cur -> size == size) {
				strncpy(cur -> file_name, file, MAX_FILENAME_LENGTH - 1);
				cur -> line_num = line;
				return ptr;
			}

			//update total_usage and total_free
			if (size > cur -> size) total_usage += size - cur -> size;
			else if (size < cur -> size) total_free += cur -> size - size;

			//first remove the memory blcok, then allocate new memory
			if (pre) pre -> next = next;
			else head = next;

			//calculate the new size of the memory block
			void *memory = realloc(cur, sizeof(meta_data) + size);
			meta_data *md = (meta_data*)memory;
			insert_meta_data(memory, size, file, line);
		
			return memory + sizeof(meta_data);
		}

		pre = cur;
		cur = next;
	}

	//can not find the memory block
	return NULL;
}

/*
 * Replace normal free, this free will also delete the node in the list.
 *
 * @param ptr
 *  Pointer to a memory block previously allocated. If a null pointer is
 *  passed, no action occurs.
 */
void mini_free(void *ptr)
{
	if (!ptr) return;

	//using the help function to remove memory block
	remove_meta_data(ptr);
}

/*
 * Helper function to insert the malloc ptr node to the list.
 * Accumulate total_usage here.
 *
 * @param md
 *  Pointer to the meta_data
 * @param size
 *  Size of the memory block, in bytes.
 * @param file
 *  Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *  Line in the file that causes memory leak
 */
void insert_meta_data(meta_data *md, size_t size, const char *file, size_t line)
{
	md -> size = size;
	md -> line_num = line;
	strncpy(md -> file_name, file, MAX_FILENAME_LENGTH - 1);

	md -> next = head;
	head = md;
}

/*
 * Helper function to remove the free ptr node from the list.
 * Add to total_free here.
 *
 * @param ptr
 *  Pointer to a memory block previously allocated.
 */
void remove_meta_data(void *ptr)
{
	meta_data *pre = NULL, *cur = head, *next = NULL;

	while (cur) {
		next = cur -> next;
		void *tmp = (void *)cur + sizeof(meta_data);
		if (tmp == ptr) {
			total_free += cur -> size;
			
			//remove the memory block
			if (pre) pre -> next = next;
			else head = next;

			free(cur);

			return;
		}

		pre = cur;
		cur = next;
	}

	//if free is double used
	++bad_frees;
}

/*
 * Deletes all nodes from the list. Called when the program exits so make sure
 * to not count these blocks as freed.
 */
void destroy()
{
	meta_data *cur = head, *next = NULL;
	while (cur) {
		next = cur -> next;
		free(cur);
		cur = next;
	}
}

/*
 * Print mini_valgrind leak report.
 */
void print_report()
{
	print_leak_info(head, total_usage, total_free, bad_frees);
}