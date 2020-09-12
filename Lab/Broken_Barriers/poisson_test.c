#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _POISSON_H
#include "poisson.h"
#endif

#ifndef _BMP_H
#define _BMP_H
#include "bmp.h"
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// This sets up the equation that you want to solve.
// If you want to solve another equation just try
// modifying what f[actual] equals. h is the just
// the spaceing used to estimate discrete points
// on the equation that we then solve.
// Remember to free the memory returned by this function
double **basic_equation(int n) {
    double *f      = calloc(sizeof(double), n * n);
    double **f_ret = malloc(sizeof(double *) * n);
    double h       = 1.0 / n;
    int y, x;
    int actual = 0;

    for (y = 0; y < n; y++) {
        f_ret[y] = &f[actual];
        for (x = 0; x < n; x++, actual++) {
          f[actual] = -200 * pow(M_PI, 2) * sin(10. * M_PI * x * h) 
                    * sin(10. * M_PI * y * h) * h * h;
        }
    }

    return f_ret;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf(
            "Usage: ./poisson_test <size> <num_threads> <granularity> <epsilon>\n\
                    The size is the number of cells in the x and y directions.\n\
                    num_threads is the number of threads to run with.\n\
                    granularity is how many iterations to wait before printing a bmp.\n\
                    epsilon is the error across the entire array that we want to achieve.\n");
        
        return 1;
    }

    int N = atoi(argv[1]);
    // int NUM_THREADS = atoi(argv[2]);
    int GRANULARITY = atoi(argv[3]);
    double EPSILON  = atof(argv[4]);
    double **f      = basic_equation(N + 2);
    
    serial_poisson(N, GRANULARITY, EPSILON, f);

    return 0;
}
