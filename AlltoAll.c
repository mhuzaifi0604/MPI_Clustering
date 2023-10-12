/*
Muhammad Huzaifa
20i-0604
Assignment # 02
Task # 01 part c All To All implementation using mpi send and mpi recieve
*/

// including necessory libraries
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char** argv){
    // Initializing MPI_Sequence
    MPI_Init(&argc, &argv);

    // Declaring variables for later use for each container
    int rank, nprocs, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Retreiving rank of process/ container b/w parallel processing
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // Retreiving number of processes of process/ container b/w parallel processing

    int start_send = nprocs * rank; // to send 1st element for each ranked process
    int end_send = start_send + 2; // to send last element for each ranked processes

    // getting size of each data elements to recieve from other processes
    int distributed_dts_size = end_send - start_send + 2;
    // initializing array to send data to slacve processes
    int *distributed_data = (int*)malloc(distributed_dts_size * sizeof(int));
    // initializing array to recieve array of size of distributed array
    int *dist_recv = (int *)malloc(distributed_dts_size * sizeof(int));

    // printing each element to send to a slave process
    for (int i = 0 ; i < distributed_dts_size; i++){
        // assigning elements to eachindex of distributed array total size / nprocs time
        distributed_data[i] = start_send + i;
        if (i == 0){ // printing assigned data in red
            printf("\x1B[31mdistributed data[%d] --> %d\x1B[33m\n", i, distributed_data[i]);
        }else if (i  == 1){ // printing assigned data in green
            printf("\x1B[32mdistributed data[%d] --> %d\x1B[33m\n", i, distributed_data[i]);
        }else if(i == 2){ // printing assigned data in yellow
            printf("\x1B[33mdistributed data[%d] --> %d\x1B[33m\n", i, distributed_data[i]);
        }else{ // printing assigned data in blue
            printf("\x1B[34mdistributed data[%d] --> %d\x1B[33m\n", i, distributed_data[i]);
        }
    }
    // appointing a barrier to assign data to and then move onto process deta recieval
    MPI_Barrier(MPI_COMM_WORLD);
    // Sending assigned element to corresponding slave 0 at process 0 index 0, 1 at process 1 at index 0 and so on
    for (int i = 0; i < nprocs; i++){
        if ( i != rank){ // sending data to all other processes except itself
            MPI_Send(&distributed_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD); // sending data
        }else{ // assigning elements to recieving array
            dist_recv[i] = distributed_data[i];
        }
    }
    // getting container ids to show code is working on clusters
    MPI_Get_processor_name(processor_name, &name_len);
    for (int i = 0; i < nprocs; i++){ // running loop for all slave processes
        if (i != rank){ // checking if not being recieved from its own process
            // recieving element at one process from other processes
            MPI_Recv(&dist_recv[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nProcess # \x1B[35m%d\x1B[0m with container Id: \x1B[31m%s\x1B[0m recieved data \x1B[32m[%d]\x1B[0m from process \x1B[35m%d\x1B[0m.\n", rank, processor_name, dist_recv[i], i);
        }
    }
    printf("\n\n");
    // deallocating dynamic memory
    free(distributed_data);
    free(dist_recv);
    // creating a barrier to wait for all processes to get their work done
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();// finalizing mpi sequence
    return 0;
}