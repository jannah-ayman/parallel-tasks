#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000;
double step;

int main()
{
    int nthreads;
    double pi;
    double sum[4]; // one slot per thread
    double start_time, end_time;

    step = 1.0 / num_steps;

    omp_set_num_threads(4);

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x;
        int id = omp_get_thread_num();      // this thread's id (0,1,2,3)
        int nthrds = omp_get_num_threads(); // actual number of threads created

        if (id == 0) nthreads = nthrds; // save actual thread count from master thread

        int i;
        // cyclic distribution: thread 0 does steps 0,4,8,... thread 1 does 1,5,9,...
        for (i = id, sum[id] = 0.0; i < num_steps; i += nthrds)
        {
            x = (i + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x); // each thread writes to its own slot
            // but sum[0] and sum[1] are neighbors in memory --> same cache line
        }
    }

    int i;
    for (i = 0, pi = 0.0; i < nthreads; i++)
        pi += sum[i] * step; // serial merge of partial sums

    end_time = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
