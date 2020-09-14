#include "company.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

volatile int running = 1; // Flag for keeping the simulator on (dont touch it!)

int delta = 100000; // Time delta for operations like trying, working, and meeting.

// Whoa! A global mutex .. I wonder what this can be used for :)
pthread_mutex_t arbitrator = PTHREAD_MUTEX_INITIALIZER;

size_t *arg_parse(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "usage: %s [num_interns] [num_companies]\n", argv[0]);
        fprintf(stderr, "OR usage: %s [num_interns] [num_companies] [delta]\n", argv[0]);
        exit(1);
    }

    char *endptr;
    long tokargs[argc - 1];
    size_t num_args = sizeof(tokargs) / sizeof(tokargs[0]);

    for (size_t i = 0; i < num_args; i++) {
        tokargs[i] = strtol(argv[i + 1], &endptr, 10);
        if (*endptr != '\0') {
            fprintf(stderr, "Failed to convert an arugment to a long!\n");
            exit(2);
        }

        if (tokargs[i] < 1) {
            fprintf(stderr, "Please have all arguments be greater than or equal to 1!\n");
            exit(3);
        }
    }

    size_t *args = malloc(num_args * sizeof(size_t));
    for (size_t i = 0; i < num_args; i++) {
        args[i] = (size_t)tokargs[i];
    }

    return args;
}

// Roughly based on code from http://rosettacode.org/
int main(int argc, char *argv[]) {
    setvbuf(stdout, 0, _IONBF, 0); // no buffering
    printf("\033[H\033[J");        // Clear

    // Arg Parsing magic
    size_t *args = arg_parse(argc, argv);

    int num_companies = args[0];
    // Exercise for the reader: what if there is only one intern?
    int num_interns   = args[1];
    if (argc == 4) {
        delta = args[2];
    }

    printf("Running simulation with %d companies and %d interns\n", num_companies, num_interns);
    srand(241); // Make rand()/scheduling somewhat repeatable

    // One mutex per intern
    // Since humans are inherently a critical section.
    pthread_mutex_t interns[num_interns];
    Company companies[num_companies];
    memset(interns, 0x00, num_interns * sizeof(pthread_mutex_t));
    memset(companies, 0x00, num_companies * sizeof(Company));

    // Initializing said mutexes
    for (int i = 0; i < num_interns; i++) {
        int failed = pthread_mutex_init(&interns[i], NULL);
        if (failed) {
            printf("Failed to initialize mutexes.");
            exit(1);
        }
    }

    // Creating the companies and assigning their interns
    for (int i = 0; i < num_companies; i++) {
        Company *company = &companies[i];
        
        Company_set_company_number(company, i);
        Company_set_left_intern(company, &interns[i % num_interns]);
        Company_set_right_intern(company, &interns[(i + 1) % num_interns]);
        Company_set_job_duration(company, rand() % delta);
        Company_set_X(company, 40 + 20 * sin((i + 0.5) * 3.14159 * 2. / num_companies));
        Company_set_Y(company, 10 + 7 * cos((i + 0.5) * 3.14159 * 2. / num_companies));
        
        printf("\033[31m\033[%d;%dH%d joined    \n", (int)Company_get_Y(company),
               (int)Company_get_X(company), Company_get_company_number(company));
        int failed_to_create_thread = pthread_create(&company->thread, NULL, work_interns, company);
        Company_set_failed_flag(company, failed_to_create_thread);
    }

    sleep(20);   // Billing cycle
    running = 0; // STOP THE SIMULATOR!

    // Telling the companies to finish whatever they are doing.
    for (int i = 0; i < num_companies; i++) {
        Company *company = &companies[i];
        if (!Company_get_failed_flag(company) && pthread_join(company->thread, NULL)) {
            // WHOOPS!
            printf("error joining thread for %d", Company_get_company_number(company));
            exit(1);
        }
    }
    printf("\033[H\033[J"); // Clear
    printf("MAKE IT RAIN!!!\n");

    // Time to collect money from companies
    int total = 0; // Number of billable days
    for (int i = 0; i < num_companies; i++) {
        Company *company = &companies[i];
        // Mo Money Mo Problems!
        printf("Company %d used our services for %d billable days.\n",
               Company_get_company_number(company),
               Company_get_billable_days(company));
        
        total += Company_get_billable_days(company);
    }
    
    printf("\nTotal Billable days : %d\n", total);
    
    return 0;
}
