#include <stdio.h>
#include "omp.h"

long long num_steps = 100000000; // number of rectangles, more = more accurate
double step;

int main()
{
    double x, sum = 0.0, pi;
    double start_time, end_time;

    step = 1.0 / num_steps; // width of each rectangle

    start_time = omp_get_wtime(); // start timer

    int i;
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;          // midpoint of rectangle
        sum += 4.0 / (1.0 + x * x);    // height of curve at midpoint
    }

    pi = sum * step; // multiply sum by width to get area = Pi

    end_time = omp_get_wtime(); // stop timer

    printf("Pi = %.10f\n", pi);
    printf("Time = %.4f seconds\n", end_time - start_time);

    return 0;
}
