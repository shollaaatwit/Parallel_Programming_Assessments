#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

extern int* imageToMat(char* name, int* dims);
extern void matToImage(char* name, int* mat, int* dims);

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank, numranks;
	MPI_Comm_size(MPI_COMM_WORLD, &numranks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int height, width;
	int remainder, remainderholder;
	double timeStart, timeEnd;

	int *sendcounts;
	int *displs;
	int *matrix;
	int *myb;
	int *dims;

	char *name = "image.jpg";
	dims = (int*)malloc(2*sizeof(int));
	sendcounts = (int*)malloc(numranks*sizeof(int));
	displs = (int*)malloc(numranks*sizeof(int));
	int *result;

	if(rank == 0)
	{
		timeStart = MPI_Wtime();
		matrix = imageToMat(name, dims);
		result = (int*)malloc(dims[0]*dims[1]*sizeof(int));
	}
	MPI_Bcast(dims, 2, MPI_INT, 0, MPI_COMM_WORLD);

	height = dims[0];
	width = dims[1];

	//allocate matrix's memory in all other ranks
	if(rank != 0)
	{
		matrix = (int*)malloc(dims[0]*dims[1]*sizeof(*matrix));
	}
	MPI_Bcast(matrix, dims[0]*dims[1], MPI_INT, 0, MPI_COMM_WORLD);

	remainder = height%numranks;
	remainderholder = 0;

	int numele = height/numranks;
	for(int i = 0; i < numranks; i++)
	{
		sendcounts[i] = numele;
		if(remainderholder != 0)
		{
			sendcounts[i] = sendcounts[i] + 1;
			remainderholder = remainderholder - 1;
		}
		displs[i] = remainderholder;
		remainderholder += sendcounts[i]; 
	}

	myb = (int*)malloc(sendcounts[rank] * width * sizeof(*matrix));
	int k = 15;

	double sum = 0;
	int index = 0;

	//process over all pixels and perform convolution
	for(int i = displs[rank]; i < (displs[rank] + sendcounts[rank]); i++)
	{

		for(int j = 0; j < width; j++)
		{
			sum = 0;
			int count = 0;
			for(int u = -k; u <= k; u++)
			{
				for(int v = -k; v <= k; v++)
				{
					if(i - u > 0 && j - v > 0 && i - u < height && j - v < width)
					{
						count++;
						sum += matrix[((i-u)*width+(j-v))];
					}
				}
			}
			myb[(index * width + j)] = sum/count; //normalization
		}
		index++;
	}

	//set new values for arrays in gatherv
	for(int i = 0; i < numranks; i++)
	{
		sendcounts[i] = sendcounts[i] * width;
		displs[i] = displs[i] * width;
	}

	MPI_Gatherv(myb, sendcounts[rank], MPI_INT, result, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

	//save image
	if(rank == 0)
	{
		timeEnd = MPI_Wtime();
		matToImage("processedImage.jpg", result, dims);

		printf("%f Time:", timeEnd - timeStart);
	}

	MPI_Finalize();
	return 0;
}
