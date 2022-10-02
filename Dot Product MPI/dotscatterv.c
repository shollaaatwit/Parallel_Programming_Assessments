#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){

	int rank, numranks;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numranks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size = 500000000; //size of vec
	double timeStart;
	double timeEnd;
	int* a;
	int* b;
	int* sendcounts;
	int* displs;
	if(rank==0)
	{
		timeStart = MPI_Wtime();
		a=(int*)malloc(size*sizeof(int));
		b=(int*)malloc(size*sizeof(int));
		sendcounts=(int*)malloc(numranks*sizeof(int));
		displs=(int*)malloc(numranks*sizeof(int));
		for(int i = 0; i < size; i++)
		{
			a[i]=b[i]=2;
		}
		for(int i = 0; i < numranks; i++)
		{
			sendcounts[i] = size/numranks; //numele value
			displs[i] = i * sendcounts[i]; //values to send  to each rank
		}
	}

	int numele = size/numranks;
	if(size%numranks != 0)
	{
		printf("must be divisible\n");
		MPI_Finalize();
		return 0;
	}
	int* mya = (int*)malloc(numele*sizeof(int));
	int* myb = (int*)malloc(numele*sizeof(int));

	MPI_Scatterv(a, sendcounts, displs, MPI_INT, mya, numele, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(b, sendcounts, displs, MPI_INT, myb, numele, MPI_INT, 0, MPI_COMM_WORLD);

	int result = 0;

	for(int i = 0; i < numele; i++)
	{
		result += mya[i] * myb[i];
	}
	MPI_Allreduce(MPI_IN_PLACE, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	if(rank == 0)
	{
		timeEnd = MPI_Wtime();
		printf("Result: %d , Time: %f Ranks: %d\n", result, timeEnd - timeStart, numranks);
	}
	MPI_Finalize();
	return 0;


}
