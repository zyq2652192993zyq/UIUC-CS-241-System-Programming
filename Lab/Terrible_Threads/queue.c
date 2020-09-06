#include "queue.h"
#include <pthread.h>
#include <stdlib.h>


/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 *  Blocks if the queue is full.
 */
void queue_push(queue_t *queue, void *data)
{
	pthread_mutex_lock(&queue -> m);

	// if the size bigger than maxSize, block
	// if maxSize <= 0, should never block
	if (queue -> maxSize > 0 && queue -> size >= queue -> maxSize)
		pthread_cond_wait(&queue -> cv, &queue -> m);

	queue_node_t *node = (queue_node_t *) malloc(sizeof(queue_node_t));

	if (!node) return;

	node -> data = data;
	node -> next = NULL;

	if (!queue -> head) queue -> head = node;

	if (!queue -> tail) queue -> tail = node;
	else {
		queue -> tail -> next = node;
		queue -> tail         = queue -> tail -> next;
	}

	++queue -> size;

	pthread_cond_broadcast(&queue -> cv);
	pthread_mutex_unlock(&queue -> m);
}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks if the queue is empty.
 */
void *queue_pull(queue_t *queue)
{
	pthread_mutex_lock(&queue -> m);

	if (queue -> size <= 0) pthread_cond_wait(&queue -> cv, &queue -> m);

	queue_node_t *node = queue -> head;
	void *data = node -> data;

	if (queue -> size == 1) {
		queue -> head = queue -> tail = NULL;
	}
	else {
		queue -> head = queue -> head -> next;
	}
	free(node);

	--queue -> size;

	pthread_mutex_unlock(&queue -> m);

	return data;
}

/**
 *  Allocates heap memory for a queue_t and initializes it.
 *  Returns a pointer to this allocated space.
 */
queue_t *queue_create(int maxSize)
{
	queue_t *queue = (queue_t *) malloc(sizeof(queue_t));

	if (!queue) return NULL;

	queue -> head    = queue -> tail = NULL;
	queue -> size    = 0;
	queue -> maxSize = maxSize;

	pthread_mutex_init(&queue -> m, NULL);
	pthread_cond_init(&queue -> cv, NULL);

	return queue;
}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(queue_t *queue)
{
	if (!queue) return;

	queue_node_t *iter = queue -> head;
	queue_node_t *cur;

	while (iter) {
		cur = iter;
		iter = iter -> next;
		free(cur);
	}

	pthread_mutex_destroy(&queue -> m);
	pthread_cond_destroy(&queue -> cv);

	free(queue);
}

