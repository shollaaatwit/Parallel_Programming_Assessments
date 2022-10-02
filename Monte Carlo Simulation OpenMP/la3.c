#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char** argv)
{
    double h = 0; //number of hits
    long n = 100000000; //total number of darts thrown
    double x; //x in predicate
    double y; ///y in predicate
    double pi;
    double startTime, endTime;
    int seed;
// #pragma omp parallel for reduction(+ : H)
    startTime = omp_get_wtime();
#pragma omp parallel
    {

        seed = calloc(1, sizeof(seed));
        seed = 3000 * omp_get_thread_num();
#pragma omp for reduction(+: h)
        for(long i = 0; i < n; i++)
        {

            x = (double)((2.0 * rand_r(&seed)) / RAND_MAX - 1);
            y = (double)((2.0 * rand_r(&seed)) / RAND_MAX - 1);
            double xSqrd = x * x;
            double ySqrd = y * y;

            if((xSqrd + ySqrd) <= 1)
            {
                h++;
            }
        }

        pi = (h / n)*4;

        endTime = omp_get_wtime();

    }
    printf("Num Thrown: %li, Num Landed: %f, Pi is %f, Error: %.12f, Time: %.12f", n, h, pi, ((acos(-1) - pi)/acos(-1)), endTime - startTime);
    return 0;
}