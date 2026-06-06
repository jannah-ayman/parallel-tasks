#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000;
double step;

int main()
{
    double sum = 0.0, pi;
    double start_time, end_time;

    step = 1.0 / num_steps;

    omp_set_num_threads(4);

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x, local_sum = 0.0; // each thread has its own local_sum, no sharing
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();

        int i;
        // each thread computes its own portion using cyclic distribution
        for (i = id; i < num_steps; i += nthrds)
        {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x); // safe -- only one thread touches local_sum
        }

        // merge into global sum, only one thread allowed here at a time
        #pragma omp critical
        {
            sum += local_sum; // serialized by critical section
        }
    }

    pi = sum * step;

    end_time = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
