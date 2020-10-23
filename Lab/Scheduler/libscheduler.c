#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libscheduler.h"

priqueue_t pqueue;
priqueue_t completedjobs;
core_t *cores;
scheme_t myscheme;

int corenum;
int job_count = 0;

int comparer_fcfs(const void *a, const void *b) 
{  
    return (((job_t*)a)->arrival_time - ((job_t*)b)->arrival_time); 
}

int comparer_pri(const void *a, const void *b) 
{ 
    if ((((job_t*)a)->priority - ((job_t*)b)->priority ) != 0) 
        return (((job_t*)a)->priority - ((job_t*)b)->priority);
    else
        return (((job_t*)a)->arrival_time - ((job_t*)b)->arrival_time);
}
int comparer_psjf(const void *a, const void *b) 
{  
    if ((((job_t*)a)->remaining_time - ((job_t*)b)->remaining_time) != 0) 
        return (((job_t*)a)->remaining_time - ((job_t*)b)->remaining_time);
    else
        return (((job_t*)a)->arrival_time - ((job_t*)b)->arrival_time);
}

int comparer_rr(const void *a, const void *b) 
{ 
    return 1; 
}

/**
  Initalizes the scheduler.

  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param numcores the number of cores that is available by the scheduler. These
  cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param s  the scheduling scheme that should be used. This value will be one of
  the six enum values of scheme_t
*/
void scheduler_start_up(int numcores, scheme_t s) 
{
    corenum = numcores;
    cores   = malloc(sizeof(core_t) *corenum);

    for (int i = 0; i < corenum; i++) cores[i].job = NULL;  

    myscheme = s;

    if (s == FCFS) {
        priqueue_init(&completedjobs, comparer_rr);
        priqueue_init(&pqueue, comparer_fcfs);
    }
    else if (s == PRI) {
        priqueue_init(&completedjobs, comparer_rr);
        priqueue_init(&pqueue, comparer_pri);
    }
    else if (s == PSJF) {
        priqueue_init(&completedjobs, comparer_rr);
        priqueue_init(&pqueue, comparer_psjf);
    }
    else if (s == RR) {
        priqueue_init(&completedjobs, comparer_rr);
        priqueue_init(&pqueue, comparer_rr);
    }
}

/**
  Called when a new job arrives.

  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it
  will be completedjobs.
  @param priority the priority of the job. (The lower the value, the higher the
  priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made.

 */
int scheduler_new_job(int job_number, int time, int running_time,
                      int priority) 
{
    job_t * new_job = malloc(sizeof(job_t));

    new_job -> job_number     = job_number;
    new_job -> priority       = priority;
    new_job -> remaining_time = running_time;
    new_job -> running_time   = running_time;


    new_job -> arrival_time  = time;
    new_job -> first_runtime = -1;
    new_job -> last_runtime  = -1;
  
    job_count++; 
    job_t *worst = NULL;
    int worst_index = -1;

    for (int i = 0; i < corenum; i++) {
        if (cores[i].job != NULL) {    
            cores[i].job->remaining_time = cores[i].job->remaining_time - (time - cores[i].job->last_runtime);
            cores[i].job->last_runtime = time;
        }
    }

    for (int i = 0; i < corenum; i++) {
        if (cores[i].job == NULL) {
            new_job -> first_runtime = time;
            new_job -> last_runtime = time;
            cores[i].job = new_job;
            return i;
        }
    }
    

    if (myscheme == FCFS || myscheme == PRI || myscheme == RR) {  
        priqueue_offer(&pqueue, new_job);
        return -1; 
    }
    else if(myscheme == PSJF) {  
        for (int i = 0; i < corenum; i++) {
            if (worst != NULL && pqueue.comparer(worst, cores[i].job) < 0) {            
                worst = cores[i].job;
                worst_index = i;
            }
            if (worst == NULL && pqueue.comparer(new_job, cores[i].job) < 0) {
                worst = cores[i].job;
                worst_index = i;
            }
        }
  
        if (worst != NULL) {
            if (worst->first_runtime == time) worst->first_runtime = -1;
            if (worst->last_runtime == time) worst->last_runtime = -1;

            new_job -> first_runtime = time;
            new_job -> last_runtime = time; 
            cores[worst_index].job = new_job;
            priqueue_offer(&pqueue, worst);
            return worst_index;
        }
  
        if (worst == NULL) {
            priqueue_offer(&pqueue, new_job);
            return -1;    
        }
    }

    return -1;
}

/**
  Called when a job has completed execution.

  The core_id, job_number and time parameters are provided for convenience. You
  may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  completedjobs job, return the job_number of the job that should be scheduled to
  run on core core_id.

  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time) 
{
    cores[core_id].job -> response_time   = cores[core_id].job -> first_runtime - cores[core_id].job -> arrival_time;
    cores[core_id].job -> turnaround_time = time - cores[core_id].job -> arrival_time;
    cores[core_id].job -> waiting_time    = time - cores[core_id].job -> arrival_time - cores[core_id].job -> running_time;    
    
    priqueue_offer(&completedjobs, cores[core_id].job);
    cores[core_id].job = priqueue_poll(&pqueue);
      
    if(cores[core_id].job != NULL) {  
        if(cores[core_id].job->first_runtime == -1)
            cores[core_id].job->first_runtime = time;

        cores[core_id].job->last_runtime = time;

        return cores[core_id].job->job_number;
    } 
    else return -1;
}

/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.

  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time) 
{
    priqueue_offer(&pqueue, cores[core_id].job);

    cores[core_id].job = priqueue_poll(&pqueue); 

    if (cores[core_id].job != NULL) {
        if (cores[core_id].job -> first_runtime == -1)
            cores[core_id].job -> first_runtime = time;

        cores[core_id].job -> last_runtime = time;  
        return cores[core_id].job -> job_number;
    }
    else return -1;
}

/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have completedjobs and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time() 
{
    float total_waiting = 0;
    entry *curr = completedjobs.head;

    while (curr != NULL) {
        total_waiting = total_waiting + (float)(((job_t*)(curr -> value)) -> waiting_time);
        curr = curr -> next;
    }

    return (float)total_waiting / ((float)job_count);
}

/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have completedjobs and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time() 
{
    float total_turnaround = 0;

    entry *curr = completedjobs.head;

    while (curr != NULL) {
        total_turnaround = total_turnaround + (float)(((job_t*)(curr->value))->turnaround_time);
        curr = curr->next;
    }

    return (float)total_turnaround/((float)job_count);
}

/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have completedjobs and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time() 
{
    float total_response = 0;
    entry *curr = completedjobs.head;

    while (curr != NULL) {
        total_response += (float)(((job_t*)(curr->value))->response_time);
        curr = curr->next;
    }

    return (float)total_response/((float)job_count);
}

/**
  Free any memory associated with your scheduler.

  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up() 
{
    for (int i = 0; i < corenum; i++) {
        if (cores[i].job != NULL) free(cores[i].job);
    }
    free(cores);

    entry *curr;
    curr = completedjobs.head;
    while (curr != NULL) {
        if (curr->value != NULL) free(curr->value);
        curr = curr->next;
    }

    curr = pqueue.head;
    while (curr != NULL) {
        if (curr->value != NULL) free(curr->value);
        curr = curr->next;
    }

    priqueue_destroy(&completedjobs);
    priqueue_destroy(&pqueue);
}

/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in
  the order they are to be scheduled. Furthermore, we have also listed the
  current state of the job (either running on a given core or idle). For
  example, if we have a non-preemptive algorithm and job(id=4) has began
  running, job(id=2) arrives with a higher priority, and job(id=1) arrives with
  a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)

  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue() {
  // This function is left entirely to you! Totally optional.
}
