#include <pthread.h>

/**
 * The struct for a Semamore.
 * It contains the value the Semaphore is initially, as well as its maximum
 * value (before it should block).
 * Also contains an associated mutex and condition variable.
 */

typedef struct {
    int value, max_val;

    pthread_mutex_t m;
    pthread_cond_t cv;

} Semamore;

// Initializes the members of the Semamore struct.
void semm_init(Semamore *s, int value, int max_val);

// Blocks when value is at 0, then decrememnts the value once it is not at 0.
void semm_wait(Semamore *s);

// Blocks when value is at max_val, then increments the value once it is not at
// max_val.
void semm_post(Semamore *s);

// Cleans up associated memory with the struct, but does not free the struct
// itself.
void semm_destroy(Semamore *s);