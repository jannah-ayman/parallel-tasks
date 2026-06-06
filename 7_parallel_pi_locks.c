#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000;
double step;

int main()
{
    double sum = 0.0, pi;
    double start_time, end_time;
    omp_lock_t lock; // declare the lock

    step = 1.0 / num_steps;

    omp_init_lock(&lock); // initialize the lock before use

    omp_set_num_threads(4);

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x, local_sum = 0.0;
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();

        int i;
        for (i = id; i < num_steps; i += nthrds)
        {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        omp_set_lock(&lock);   // acquire the lock, other threads block here until released
        sum += local_sum;      // safe, only one thread in here at a time
        omp_unset_lock(&lock); // release the lock so next thread can enter
    }

    pi = sum * step;

    end_time = omp_get_wtime();

    omp_destroy_lock(&lock); // clean up the lock after we are done

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
