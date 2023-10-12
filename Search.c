/*
Muhammad Huzaifa
20i-0604
Assignment # 02
Task # 02 Searching Element Using All Slaves
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include<time.h>

#define ARRAY_SIZE 100
#define Target 12

int main(int argc, char *argv[]) {
    // Initializing MPI_Sequence
    MPI_Init(&argc, &argv);

    // Declaring variables for later use for each container
    int rank, size, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Retreiving rank of process/ container b/w parallel processing
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Retreiving size of process/ container b/w parallel processing

    int distributed_size = ARRAY_SIZE / (size - 1); // Getting size for the distributed array to send to each slave
    int distributed_array[distributed_size]; // creating array to receive distributed data in

    if (rank == 0) { // What master is supposed to do
        // Initialize the global array with random values (only done by the master).
        int global_array[ARRAY_SIZE];
        for (int i = 0; i < ARRAY_SIZE; i++) {
            global_array[i] = i+1; // initializing element from 1 to 100 to distribute
        }
        for (int i = 1; i < size; i++) {
            int dest_rank = i;
            int start_idx = (i - 1) * distributed_size;
            // Sending distributed array to each slave using mpi_send
            MPI_Send(&global_array[start_idx], distributed_size, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
        }
    } else { // what all slaves are supposed to do

        // Slave processes receive their parts of the array.
        MPI_Recv(distributed_array, distributed_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // getting container id to show code is working on containers
        MPI_Get_processor_name(processor_name, &name_len);
        // Priting distributed array recieved by each slave
        printf ("\n\x1B[33mProcess with Rank #  %d\x1B[0m, Container ID # \x1B[31m%s\x1B[0m \x1B[35m-->\x1B[0m", rank, processor_name);
        for (int i = 0 ; i < distributed_size; i++){
            printf("\x1B[34m%d\x1B[0m, ", distributed_array[i]); // printing elements recieved by the ranked process
            if (i == distributed_size -1){
                printf ("\n"); // moving to next line at the end of loop
            }
        }
        printf ("\n");

        // The slave process searches its part of the array.
        int found = 0, abort_signal = 0; // checks for aborting search and finding element
        for (int i = 0; i < distributed_size; i++) { // running loop for each process till size of each process recieved array
            if (distributed_array[i] == Target) { // if found target
                found = 1;
                printf("\x1B[32mSlave process\x1B[0m \x1B[33m%d\x1B[0m \x1B[32mfound the target number.\x1B[0m\n", rank);
                MPI_Send(&found, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); // sending master signal that ranked process have found the target number
                //break;
            }
            // using mpi probe to check if master has sent abort signal to stop searching
            MPI_Iprobe(0, 2, MPI_COMM_WORLD, &abort_signal, MPI_STATUS_IGNORE);
            if(abort_signal){ // if master has sent signal
                printf("\x1B[34mAborting Search at process\x1B[0m # \x1B[33m%d\x1B[0m\n", rank);
                break; // then abort
            }
        }
    }
    // using rank == 0 again as master to send abort signal after recieving found
    if (rank == 0){
        int num_found = 0;
        // checking if any procvess has sent that num to search hs been found
        MPI_Recv(&num_found, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (num_found){ // sending abort signal if  um has been found
            int abort = 1;
            printf("\nTarget Found - Sending Abort Signal\n");
            for (int i = 1; i < size; i++){ // sending abort signal to each process
                MPI_Send(&abort, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
        }
    }
    // Finalizing the MPI sequence
    MPI_Finalize();
    return 0;
}

