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
        double x, local_sum = 0.0; // private to each thread
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();

        int i;
        for (i = id; i < num_steps; i += nthrds)
        {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        // atomic protects a single read-modify-write operation at the hardware level
        // faster than critical but only works for simple operations like +=
        #pragma omp atomic
        sum += local_sum;
    }

    pi = sum * step;

    end_time = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
