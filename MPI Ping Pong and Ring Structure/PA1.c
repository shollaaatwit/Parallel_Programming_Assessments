#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){

    int numranks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);


    int *num = (int*)malloc(20000000 * sizeof(int));
    int counter = 2;
    int amount = 20000000;
    double timeStart;
    double timeEnd;
    MPI_Status status;

    //Ring Structure
    if(rank == 0) //base case
    {

        printf("Ring Structure with %d nodes\n", numranks);

        timeStart = MPI_Wtime(); //time at start of ring
        MPI_Send(num, amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("(Ring) Sending to Rank %d \n", rank+1);

        MPI_Recv(num, amount, MPI_INT, numranks-1, 0, MPI_COMM_WORLD, &status);
        timeEnd = MPI_Wtime(); //time at end of ring
        printf("(Ring) Receiving from Rank %d with time %f \n", numranks, timeEnd-timeStart);
        
    }
    if(rank > 0 && rank < numranks)
    {

        MPI_Recv(num, amount, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("(Ring) Receiving from Rank %d \n", rank-1);

        if(numranks-1 == rank)
        {

            printf("(Ring) Sending to Rank 0 \n");
            MPI_Send(num, amount, MPI_INT, 0, 0, MPI_COMM_WORLD); //send back to 0 at end of ring

        }
        else
        {

            printf("(Ring) Sending to Rank %d \n", rank+1); //increment rank if its not at the end
            MPI_Send(num, amount, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

        }

    }
    //end ring structure


    //Ping Pong Structure
    if(rank == 0)
    {
        timeStart = MPI_Wtime();
        printf("Ping Pong Structure with %d nodes\n", numranks);
        for(int i = 1; i < counter; i++)
        {
            printf("(Ping Pong) Sending to Rank %d \n", rank);
            MPI_Send(num, amount, MPI_INT, i, 0, MPI_COMM_WORLD);

            if(counter != numranks)
            {
                counter++;
            }

            printf("(Ping Pong) Receiving from Rank %d \n", rank);
            MPI_Recv(num, amount, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
           
        }

        timeEnd = MPI_Wtime();
        printf("(Ping Pong) Receiving from Rank %d with time %f \n", numranks, timeEnd-timeStart);
    }
    else
    {
        printf("(Ping Pong) Receiving from Rank %d \n", rank);
        MPI_Recv(num, amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("(Ping Pong) Sending to Rank %d \n", rank);   
        MPI_Send(num, amount, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    //end ping


    free(num);
    MPI_Finalize();
}
