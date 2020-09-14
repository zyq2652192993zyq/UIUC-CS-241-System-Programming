#include "company.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _GNU_SOURCE

extern pthread_mutex_t arbitrator;

void *work_interns(void *p) 
{
	Company *company = (Company *)p;
	pthread_mutex_t *left_intern, *right_intern;

 	while (running) {
        right_intern = Company_get_right_intern(company);
        left_intern  = Company_get_left_intern(company);
 		
        if (left_intern == NULL || right_intern == NULL || left_intern == right_intern) {
        	Company_have_board_meeting(company);
        }
		else {
        	if(left_intern >= right_intern) {
        		pthread_mutex_lock(right_intern);
        		pthread_mutex_lock(left_intern);
        	}
        	else {
        		pthread_mutex_lock(left_intern);
        		pthread_mutex_lock(right_intern);
        	}
            
        	Company_hire_interns(company);
	        pthread_mutex_unlock(right_intern);
	        pthread_mutex_unlock(left_intern);
        	Company_have_board_meeting(company); 
        }
	}

	return NULL;
}