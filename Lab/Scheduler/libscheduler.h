#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIBSCHEDULER_H_
#define LIBSCHEDULER_H_

#include "libscheduler.h"
#include "libpriqueue.h"

typedef struct _job_t job_t;
typedef struct _core_t core_t;

/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job
  queue elements.
*/
struct _job_t 
{
    int arrival_time;
    int first_runtime;
    int last_runtime;
    int running_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int priority;
    int job_number;
    int remaining_time;

  /* Add whatever other bookkeeping you need into this struct. */
};

struct _core_t {
    int free;   // boolean representing whether the core is in use or not
    job_t *job; // the job that the core is currently running, if any
};

/**
  Constants which represent the different scheduling algorithms
*/
typedef enum {
    FCFS = 0,
    PRI,
    RR,
    PSJF
} scheme_t;

int comparer_psjf(const void *a, const void *b);
int comparer_sjf(const void *a, const void *b);
int comparer_pri(const void *a, const void *b);
int comparer_fcfs(const void *a, const void *b);
int comparer_rr(const void *a, const void *b);

void scheduler_start_up(int numcores, scheme_t s);
int scheduler_new_job(int job_number, int time, int running_time, int priority);
int scheduler_job_finished(int core_id, int job_number, int time);
int scheduler_quantum_expired(int core_id, int time);
float scheduler_average_turnaround_time();
float scheduler_average_waiting_time();
float scheduler_average_response_time();
void scheduler_clean_up();

void scheduler_show_queue();

#endif /* LIBSCHEDULER_H_ */
