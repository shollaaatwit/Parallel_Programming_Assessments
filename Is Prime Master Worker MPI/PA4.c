#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(int n);

int main(int argc, char** argv){

    //initializing MPI
    MPI_Init(&argc, &argv);
    int rank, numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request;
    MPI_Status stat;

    double timeStart, timeEnd;
    int n=25000000;
    int start = 0;
    int end = 0;
    // int work = n/(numranks-1); //even load balancing
    int work = 50000; //fixed work balancing 
    int current = 0;
    int numprimes = 0;
    int numeles = n/(numranks - 1);
    


    if(rank == 0) // Master 
    {
        timeStart = MPI_Wtime();
        for(int i = 1; i < numranks; i++)
        {
            start = current;
            end = current + work - 1;
            current += work;

            MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int done = 0;
        // for(int i = 1; i < numranks; i++)
        while(1)
        {
            int someNumPrimes = 0;
            MPI_Recv(&someNumPrimes, 1, MPI_INT, MPI_ANY_SOURCE,
                                        0, MPI_COMM_WORLD, &stat);
            numprimes += someNumPrimes;

            if(current == -1)
            {
                start = -1;
                done++;
            }
            else
            {
                start = current;
                end = current + work - 1;
                current += work;
            }
            if(end >= n)
            {
                end = n;
                current = -1;
            }

            MPI_Send(&start, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);

            if(done == numranks -1)
            {
                break;
            }
        }
        timeEnd = MPI_Wtime();
        printf("Number of Primes: %d\n", numprimes);
        printf("Time: %f\n", timeEnd - timeStart);
    }

    if(rank != 0) // Worker
    {
        while(1)
        {
            MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
            MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

            if(start == -1)
            {
                break;
            }
            numprimes = 0;
            for(int i = start; i <= end; i++)
            {
                if (is_prime(i) == 1)
                {
                    numprimes++;
                }
            }
            MPI_Send(&numprimes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}

//master/worker example on the slides
//split up work evenly amongst sends
//return back increments and add them all back together at
//master class
int is_prime(int n)
{
    /* handle special cases */
    if      (n == 0) return 0;
    else if (n == 1) return 0;
    else if (n == 2) return 1;

    int i;
    for(i=2;i<=(int)(sqrt((double) n));i++)
        if (n%i==0) return 0;

    return 1;
}
