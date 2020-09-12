#ifndef _BARRIER_H
#define _BARRIER_H
#include "barrier.h"
#endif
#ifndef _BMP_H
#define _BMP_H
#include "bmp.h"
#endif

typedef struct poisson_struct {
    // The solution array we are currently calculating.
    double **current;
    // The solution array from the previous iteration.
    double **previous;
    // The function we are solving the poisson equation for.
    double **target_image;
    // The number of iterations we have run.
    int iter;
    // The grid size we are solving over.
    int n;
    // The number of threads we are using to solve.
    int num_threads;
    // The number of iterations between bmp writes.
    int granularity;
    // The error over the entire grid that we use as a threshold for our solver
    double epsilon;
} poisson_struct;

int serial_poisson(int n, int granularity, double epsilon,
                   double **target_image);
int parallel_poisson(int n, int granularity, double epsilon,
                     double **target_image, int num_threads);

// Returns an array of poisson_structs, one for each thread which all point to
// the same
// nxn arrays.
poisson_struct **poisson_setup(int n, double **target_image, int num_threads,
                               int granularity, double epsilon);
// Destroys the nxn arrays set up in poisson_setup.
void poisson_destroy(poisson_struct **ps_array, int num_threads);
