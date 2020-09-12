#include "queue.h"
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

void *producer1(void *para)
{
	queue_t *queue = (queue_t *)para;

	for (int i = 0; i < 5; ++i) {
		queue_push(queue, (void *)&i);
	}

	return (void *)NULL;
}

void *producer2(void *para)
{
	queue_t *queue = (queue_t *)para;

	for (int i = 0; i < 5; ++i) {
		queue_push(queue, (void *)&i);
	}

	return (void *)NULL;
}

void *consumer1(void *para)
{
	queue_t *queue = (queue_t *)para;

	for (int i = 0; i < 5; ++i) {
		queue_pull(queue);
	}

	return (void *)NULL;
}

void *consumer2(void *para)
{
	queue_t *queue = (queue_t *)para;

	for (int i = 0; i < 5; ++i) {
		queue_pull(queue);
	}

	return (void *)NULL;
}


int main()
{
	queue_t *queue = queue_create(10);

	pthread_t tid[4];
	pthread_create(&tid[0], NULL, producer1, (void *)queue);
	pthread_create(&tid[2], NULL, consumer1, (void *)queue);
	pthread_create(&tid[1], NULL, producer2, (void *)queue);
	pthread_create(&tid[3], NULL, consumer2, (void *)queue);

	for (int i = 0; i < 4; ++i) {
		pthread_join(tid[i], NULL);
	}

	assert(queue -> size == 0);
	printf("%s\n", "All tests passed!");

	queue_destroy(queue);

	return 0;
}