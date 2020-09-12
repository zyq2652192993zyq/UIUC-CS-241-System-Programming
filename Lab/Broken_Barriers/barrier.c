#include "barrier.h"

int barrier_destroy(barrier_t *barrier)
{
	pthread_mutex_destroy(&barrier -> mtx);
	pthread_cond_destroy(&barrier -> cv);

	return 0;
}

int barrier_init(barrier_t *barrier, unsigned num_threads)
{
	pthread_mutex_init(&barrier -> mtx, NULL);
	pthread_cond_init(&barrier -> cv, NULL);

	barrier -> n_threads  = num_threads;
	barrier -> count      = 0;
	barrier -> times_used = 0;

	return 0;
}

int barrier_wait(barrier_t *barrier)
{
	pthread_mutex_lock(&barrier -> mtx);

	unsigned int new_n_threads = barrier -> times_used;

	++barrier -> count;

	if (barrier -> count == barrier -> n_threads) {
		++barrier -> times_used;
		barrier -> count = 0;
		pthread_cond_broadcast(&barrier -> cv);
	}
	else {
		while (barrier -> times_used == new_n_threads) pthread_cond_wait(&barrier -> cv, &barrier -> mtx);
	}

	pthread_mutex_unlock(&barrier -> mtx);

	return 0;
}