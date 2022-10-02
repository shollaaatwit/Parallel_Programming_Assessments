#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <mpi.h>

extern void matToImage(char* filename, int* mat, int* dims);

int main(int argc, char **argv){

    int rank, numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    //resolution
    int col = 6000; //nx
    int row = 4000; //ny

    int maxiter = 255;

    int *mat;

    //imag nums to use in Z = Z^2+C
    double cx;
    double cy;
    double x;
    double y;

    double r_start = -2;
    double r_end = 1;
    double i_start = -1;
    double i_end = 1;

    int iter;

    int numoutside = 0;
    mat = (int*)malloc(col*row*sizeof(int));
    int* sendcounts = (int*)malloc(numranks*sizeof(int));
    int* displs = (int*)malloc(numranks*sizeof(int));


    double start, end;
    double ompstart, ompend;
    int amount = row/numranks;
    int startpoint = rank * amount;
    int endpoint = startpoint + amount;

    if(rank == 0)
    {
        start = MPI_Wtime();
    }
    if (rank == numranks)
    {
        startpoint = startpoint + (amount % numranks);
        amount = amount + (amount % numranks);
    }


    int temp = (col*row)/numranks;
    
    for(int i = 0; i < numranks; i++)
    {
        sendcounts[i] = temp;
        displs[i] = temp * i;
    }
    
    MPI_Bcast(mat, (col*row), MPI_INT, 0, MPI_COMM_WORLD);



    #pragma omp parallel for reduction(+:x,y,cx,cy,iter,numoutside)
        for(int i = startpoint; i < endpoint; i++)
        {
            for(int j = 0; j < col; j++)
            {
                cx = r_start + 1.0 * j/col * (r_end - r_start);
                cy = i_start + 1.0 * i/row * (i_end - i_start);

                x = 0;
                y = 0;
                iter = 0;

                while(iter < maxiter)
                {
                    iter++;
                    double oldx = x;
                    oldx = x;
                    x = x*x-y*y+cx;
                    y = 2*oldx*y+cy;

                    if(x*x+y*y > 4)
                    {
                        numoutside++;
                        break;
                    }
                }
                mat[(i - startpoint) * col + j] = iter;
            }

        }

        end = MPI_Wtime();
        printf("Rank %d Points: %d, %d\n", rank, startpoint, endpoint);


        int* result = (int*)malloc(col*row*sizeof(int));

        MPI_Gatherv(mat, sendcounts[rank], MPI_INT, result, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Finalize();

        int dims[2] = {row, col};
        if(rank == 0)
        {
            matToImage("mandelbrot.jpg", result, dims);
            printf("Total Time: %.5f \n", end-start);
        }
        return(0);
}
