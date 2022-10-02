#include <stdio.h>
#include <stdlib.h>
#include "mvp-student.h"

//Main function
int main(int argc, char **argv)
{
	double *mat;
	double *vec;
	double *finalArray;

	int row = 4;
	int col = 4;

	mat = (double*)malloc((row*col) * sizeof(double));
	vec = (double*)malloc((row) * sizeof(double));

	assignMatrix(mat, 4, 4);
	assignMatrix(vec, 4, 1);

	finalArray = mvp(mat, vec, 4, 4);

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			printf("%f\n", mat[i * col + j]);
		}
	}
	printf("\n");


	for(int i = 0; i < row; i++)
	{
		printf("%f\n", vec[i]);
	}

	printf("\n");
	for(int i = 0; i < row; i++)
	{
		printf("%f\n", finalArray[i]);
	}


	free(mat);
	free(vec);
	free(finalArray);

	return 0;
}

double* mvp(double* mat, double* vec, int n, int m)
{

	double *resultArray;

	resultArray = (double*)malloc(n * sizeof(double));

	for(int i = 0; i < n; i++) // row #
	{

		for(int j = 0; j < m; j++) // column #
		{

			resultArray[i] += mat[i * m + j] * vec[i]; // ex: matrix 4x4 vector size 4, resultArray[1] = (mat[1 : 4] * vec[1]) + resultArray[1] ( param = <0> ) for first

		}

	}

	return resultArray;

}


void assignMatrix(double* mat, int n, int m)
{
	int arrayDim = n * m;
	

	//fills arrays with 0
	for(int i = 0; i < n; i++)
	{

		for(int j = 0; j < m; j++)
		{

			if(i == j)
			{

				mat[i * m + j] = 3;

			}
			else if(i == j-1 || i-1 == j)
			{

				mat[i * m + j] = 2;

			}
			else
			{

				mat[i * m + j] = 1;

			}

		}

	}

}
