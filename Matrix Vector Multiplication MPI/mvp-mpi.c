#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
// #include "mvp-mpi.h"

//Main function
int main(int argc, char **argv)
{
    int rank, numranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int *mat = NULL;
	int *vec = NULL;
	int *finalArray;
	int *mya;
	int *myb;
	double timeStart;
	double timeEnd;
	int row = 10;
	int col = 10;
    // rank 0 allocates arrays and values


    if(rank == 0)
    {
        mat = (int*)malloc((row*col) * sizeof(int));
        // assignMatrix(mat, row, col); //mvp method not working correctly


		// Assigns Matrix Values to all indices of the initial Matrix ex: 	[2, 1, 0]
		//																	[1, 2, 1]
		//																	[0, 1, 2]
		for(int i = 0; i < row; i++)
		{

			for(int j = 0; j < col; j++)
			{

				if(i == j)
				{
					
					mat[i * col + j] = 2;

				}
				else if(i == j-1 || i-1 == j)
				{

					mat[i * col + j] = 1;

				}
				else
				{

					mat[i * col + j] = 0;

				}

			}
			timeStart = MPI_Wtime();

		}


		// Assigns Vector Values to all indices ex: [0, 1, 2]
		vec = (int*)malloc((row) * sizeof(int));
		for(int i = 0; i < row; i++)
		{

			vec[i] = 2;

		}


		// Allocating memory for the final array of size row
		finalArray = (int*)calloc((row) , sizeof(int));

    } // end rank 0 if statement



	mya = (int*)calloc(row/numranks * col, sizeof(int));


	//If vec is null assign int values to all
	if(vec == NULL)
	{

		vec = (int*)malloc(row*sizeof(int));

	}
	if(mat == NULL)
	{

		mat = (int*)malloc(row*col*sizeof(int));

	}


	
	myb = (int*)calloc(row/numranks, sizeof(int));
	//Scattering matrix array to nodes and broadcasting vector to all ranks
	MPI_Bcast(vec, row, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(mat, row/numranks * col, MPI_INT, mya, row/numranks * col, MPI_INT, 0, MPI_COMM_WORLD);
	//calculations
	// myb = mvp(mya, vec, row/numranks, col);


	for(int i = 0; i < row/numranks; i++) // row #
	{
		for(int j = 0; j < col; j++) // column #
		{
			myb[i] += mya[i * col + j] * vec[j]; 
		}
	}
	MPI_Gather(myb, row/numranks, MPI_INT, finalArray, row/numranks, MPI_INT, 0, MPI_COMM_WORLD);



	//gather all info to rank 0
	if(rank == 0)
	{
		// printing final array
		for(int i = 0; i < row; i++)
		{
			printf("%i\n", finalArray[i]);
		}
		free(mya);
		free(myb);
		free(vec);
		free(mat);
		free(finalArray);
		timeEnd = MPI_Wtime();
		printf("%f Time for %i Ranks, Row Size %i and Col Size %i\n", timeEnd - timeStart, numranks, row, col);

	}


	MPI_Finalize();

}
