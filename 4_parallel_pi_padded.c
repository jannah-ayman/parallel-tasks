#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000;
double step;

#define PAD 8 // 64-byte cache line / 8-byte double = 8 --> one slot per cache line

int main()
{
    int nthreads;
    double pi;
    double sum[4][PAD]; // sum[id][0] is the value, the rest is padding to push next slot to next cache line
    double start_time, end_time;

    step = 1.0 / num_steps;

    omp_set_num_threads(4);

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x;
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();

        if (id == 0) nthreads = nthrds;

        int i;
        // same cyclic distribution as before
        for (i = id, sum[id][0] = 0.0; i < num_steps; i += nthrds)
        {
            x = (i + 0.5) * step;
            sum[id][0] += 4.0 / (1.0 + x * x);
            // now sum[0][0] and sum[1][0] are 8 doubles = 64 bytes apart
            // they are on different cache lines --> no false sharing
        }
    }

    int i;
    for (i = 0, pi = 0.0; i < nthreads; i++)
        pi += sum[i][0] * step;

    end_time = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
