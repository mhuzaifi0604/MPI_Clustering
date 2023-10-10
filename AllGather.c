/*
Muhammad Huzaifa
20i-0604
Assignment # 02
Task # 01 Part 01 === AllGather using MPI_send & MPI_Recv
*/

// Inclusing necessory libraries
#include <mpi.h>
#include <stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    // declaring variable one time outside any clustered env
    int rank, size, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    
    // Initializing MPI Sequence 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Retreiving rank of process/ container b/w parallel processing
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Retreiving size of process/ container b/w parallel processing
    
    //initializing data to send with the rank of the process
    int data_to_send = rank;
    int *data_to_recv = (int*)malloc(size*sizeof(int)); // creating a dynamic buffer to recv data

    // Running loop for all processes/ containers => sending data to all other slaves
    for(int i = 0 ; i < size; i++){
        if(i != rank){// Sending data if rank is not processes own rank 
            // Sending data to another process (rank == i) -> for each container
            MPI_Send(&data_to_send, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            // Recieving data from all other processes in recv buffer
            MPI_Recv(&data_to_recv[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else{
            data_to_recv[i] = data_to_send; // data to recv is same if rank is processes' own
        }
    }
    // getting container id to show code is working on containers
    MPI_Get_processor_name(processor_name, &name_len);
    // Printing data recieved to each process
    printf("\n\t\t \x1B[35m==========\x1B[0m Process with Rank # \x1B[33m%d\x1B[0m, Container ID # \x1B[31m%s\x1B[0m \x1B[35m========== \x1B[0m\n", rank, processor_name);
    printf("Data recieved: [");
    for(int i = 0 ; i < size; i++){
        printf("\x1B[32m%d\x1B[0m ", data_to_recv[i]); // printing data recieved by each process
    }
    printf("]\n");
    free(data_to_recv); // deallocating the memory allocated to recieve buffer
    MPI_Finalize(); // finallizing the MPI sequence
    return 0;

}