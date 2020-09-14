#include "company.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>

void Company_hire_interns(Company *company) {
    printf("\033[32m\033[%d;%dH%d is trying    \n", (int)Company_get_Y(company),
         (int)Company_get_X(company), Company_get_company_number(company));
    
    usleep(rand() % delta); // Sleep for sometime to visualize company trying to get intern

    pthread_mutex_t *left_intern  = Company_get_left_intern(company);
    pthread_mutex_t *right_intern = Company_get_right_intern(company);

    pthread_t company_thread = syscall(SYS_gettid);
    pthread_t left_owner     = left_intern -> __data.__owner;
    pthread_t right_owner    = right_intern -> __data.__owner;

    if (!left_owner || !right_owner) {
        printf("Both interns need to be locked before you hire them!\n");
        exit(1);
    }
    if (right_owner != left_owner) {
        printf("Both interns need to be locked by the SAME company before you hire them!\n");
        exit(1);
    }
    if (company_thread != right_owner || company_thread != left_owner) {
        printf("The company that has locked the interns must be the one to hire them!\n");
        exit(1);
    }

    printf("\033[35m\033[%d;%dH%d is working    \n", (int)Company_get_Y(company),
         (int)Company_get_X(company), Company_get_company_number(company));
    
    usleep(Company_get_job_duration(company));
    
    company->billable_days++;
    if (rand() % 2 == 0) {
        // After working a company might want to have a board meeting.
        Company_have_board_meeting(company);
    }
}

void Company_have_board_meeting(Company *company) {
    printf("\033[31m\033[%d;%dH%d is meeting    \n", (int)Company_get_Y(company),
         (int)Company_get_X(company), Company_get_company_number(company));
    usleep(rand() % delta);
}

pthread_mutex_t *Company_get_left_intern(Company *company) {
    return company -> left_intern;
}

void Company_set_left_intern(Company *company, pthread_mutex_t *left_intern) {
    company -> left_intern = left_intern;
}

pthread_mutex_t *Company_get_right_intern(Company *company) {
    return company -> right_intern;
}

void Company_set_right_intern(Company *company, pthread_mutex_t *right_intern) {
    company -> right_intern = right_intern;
}

pthread_t *Company_get_thread(Company *company) { return &company->thread; }

int Company_get_failed_flag(Company *company) { return company->failed; }

void Company_set_failed_flag(Company *company, int failed) {
    company -> failed = failed;
}

int Company_get_company_number(Company *company) {
    return company -> company_number;
}

void Company_set_company_number(Company *company, int company_number) {
    company -> company_number = company_number;
}

int Company_get_job_duration(Company *company) { return company->job_duration; }

double Company_get_X(Company *company) { return company -> x; }

void Company_set_X(Company *company, double x) { company -> x = x; }

double Company_get_Y(Company *company) { return company -> y; }

void Company_set_Y(Company *company, double y) { company -> y = y; }

void Company_set_job_duration(Company *company, int job_duration) {
    company -> job_duration = job_duration;
}

int Company_get_billable_days(Company *company) {
    return company -> billable_days;
}
