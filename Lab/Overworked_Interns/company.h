#ifndef __COMPANY_H__
#define __COMPANY_H__
#include <pthread.h>

/**
 * This flag is initialized in the simulator,
 * so we are externing it to make it visable.
 */
extern volatile int running;

/**
 * This flag is used to determine how long operations like
 * trying to work, working, and meeting take.
 */
extern int delta;

/**
 * This is the struct for a Company object,
 * but you should not modify it directly.
 * Think of all the member variables as being private.
 * So you must call on the public API.
 */
typedef struct Company {
	pthread_mutex_t *left_intern, *right_intern;
	int company_number;
	pthread_t thread;
	int job_duration;
	int failed;
	int billable_days;
	int x, y; // Screen position
} Company;

/**
 * This is the start_routine that the company uses to grab
 * the intern to the left and right.
 */
void *work_interns(void *p);

/**
 * Call this function once you have locked both the left and right intern.
 * This is will increment 'company' billable_days.
 */
void Company_hire_interns(Company *company);

/**
 * Call this function so that your company can have a board meeting.
 * Remember: A company can only be working / trying to work or having a board
 * meeting.
 */
void Company_have_board_meeting(Company *company);

/**
 * Return a pointer to the left intern
 */
pthread_mutex_t *Company_get_left_intern(Company *company);

/**
 * Sets the pointer to the left intern
 */
void Company_set_left_intern(Company *company, pthread_mutex_t *left_intern);

/**
 * Return a pointer to the right intern
 */
pthread_mutex_t *Company_get_right_intern(Company *company);

/**
 * Sets the pointer to the right intern
 */
void Company_set_right_intern(Company *company, pthread_mutex_t *right_intern);

/**
 * Returns the company number of 'company'.
 */
int Company_get_company_number(Company *company);

/**
 * Sets the company number of 'company'.
 */
void Company_set_company_number(Company *company, int company_number);

/**
 * Returns the failed flag.
 */
int Company_get_failed_flag(Company *company);

/**
 * Sets the failed flag.
 */
void Company_set_failed_flag(Company *company, int fail);

/**
 * Sets how long a job at the company takes
 */
void Company_set_job_duration(Company *company, int job_duration);

/**
 * Gets the screen position X coordinate
 */
double Company_get_X(Company *company);

/**
 * Sets the screen position X coordinate
 */
void Company_set_X(Company *company, double x);

/**
 * Gets the screen position Y coordinate
 */
double Company_get_Y(Company *company);

/**
 * Sets the screen position y coordinate
 */
void Company_set_Y(Company *company, double y);

/**
 * Call this function once you have locked both the left and right intern.
 * This is will increment 'company' billable_days.
 */
void Company_hire_interns(Company *company);

/**
 * Returns a pointer to the thread that represents the company
 */
pthread_t *Company_get_thread(Company *company);

/**
 * Returns the failed flag.
 */
int Company_get_failed_flag(Company *company);

/**
 * Sets the failed flag.
 */
void Company_set_failed_flag(Company *company, int fail);

/**
 * Returns how long a job at the company takes
 */
int Company_get_job_duration(Company *company);

/**
 * Sets how long a job at the company takes
 */
void Company_set_job_duration(Company *company, int job_duration);

/**
 * Returns how many billable days the company has.
 */
int Company_get_billable_days(Company *company);

#endif
