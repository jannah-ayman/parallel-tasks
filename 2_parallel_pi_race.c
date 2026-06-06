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

    int i;
    // all 4 threads run this loop and all write to the same 'sum' at the same time
    // all thread reads sum and all add their value and write back
    // one write overwrites the other --> lost updates --> wrong answer
    #pragma omp parallel for
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x); // RACE CONDITION HERE -- no protection on sum
    }

    pi = sum * step;

    end_time = omp_get_wtime();

    printf("Pi = %.10f  (expected ~3.1415926536)\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);
    return 0;
}
