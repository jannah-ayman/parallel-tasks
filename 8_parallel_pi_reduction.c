#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000;
double step;

int main()
{
    double x, sum = 0.0, pi;
    double start_time, end_time;

    step = 1.0 / num_steps;

    omp_set_num_threads(4);

    start_time = omp_get_wtime();

    // reduction(+:sum) handles everything automatically:
    // it gives each thread a private copy of sum initialized to 0
    // and then merges all copies using + at the end of the parallel region

    // no race condition, no manual arrays, no synchronization code needed
    #pragma omp parallel for private(x) reduction(+:sum)
    for (int i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x); // each thread writes to its own private copy of sum
    }                                 // at the end all copies are added together automatically

    pi = sum * step;

    end_time = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
