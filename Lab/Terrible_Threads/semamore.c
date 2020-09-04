#include "semamore.h"

// Initializes the members of the Semamore struct.
void semm_init(Semamore *s, int value, int max_val)
{
	s -> value   = value;
	s -> max_val = max_val;

	pthread_mutex_init(&(s -> m), NULL);
	pthread_cond_init(&(s -> cv), NULL);
}

// Blocks when value is at 0, then decrememnts the value once it is not at 0.
void semm_wait(Semamore *s)
{
	pthread_mutex_lock(&(s -> m));

	if (s -> value <= 0) pthread_cond_wait(&(s -> cv), &(s -> m));
	s -> value -= 1;
	pthread_cond_broadcast(&(s -> cv));

	pthread_mutex_unlock(&(s -> m));
}

// Blocks when value is at max_val, then increments the value once it is not at
// max_val.
void semm_post(Semamore *s)
{
	pthread_mutex_lock(&(s -> m));

	if (s -> value >= s -> max_val) pthread_cond_wait(&(s -> cv), &(s -> m));
	s -> value += 1;
	pthread_cond_broadcast(&(s -> cv));

	pthread_mutex_unlock(&(s -> m));
}

// Cleans up associated memory with the struct, but does not free the struct
// itself.
void semm_destroy(Semamore *s)
{
	pthread_mutex_destroy(&(s -> m));
	pthread_cond_destroy(&(s -> cv));
}