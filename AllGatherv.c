/*
Muhammad Huzaifa
20i-0604
Assignment # 02
Task # 01 Part 02 === AllGatherv using MPI_send & MPI_Recv
*/

// Including Necessory Libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    // Initializing MPI sequence
    MPI_Init(&argc, &argv);
    // Declaring variables for later use for each container
    int rank, size, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Retreiving rank of process/ container b/w parallel processing
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Retreiving size of process/ container b/w parallel processing

    // DDefining variable sixe of data to be sent to each slave
    int variable_data_size = rank + 1;
    int* data_to_send = (int*)malloc(variable_data_size * sizeof(int)); // initializing memory fior data to send using size calculated

    // Prinitng the data sent by each process of variable size
    printf("\n\n \x1B[33mPrinting my data array woth size\x1B[0m: \x1B[31m%d\x1B[0m\n\n", variable_data_size);
    for (int i = 0; i < variable_data_size; i++) {
        data_to_send[i] = (rank + 1) * 10 + i; // adding data to send data variable (each time of diff size) since each time data size = rank + 1 
        printf("\x1B[32m%d\x1B[0m ", data_to_send[i]); // printing the elements of data to be sent 
    }
    printf("\nDone printing Data of length %d", variable_data_size);

    // Declaring vars and arrays to keep track of elements sent by each process, places to store data in recv buffer and total data sent by the slaves
    int total_data_recved = 0;
    int* var_counts = (int*)malloc(size * sizeof(int));
    int* places = (int*)malloc(size * sizeof(int));

    // Determine the total number of elements to be received by each process
    for (int i = 0; i < size; i++) {
        var_counts[i] = i + 1; // calculating total data to be recved in accordance to rank + 1
        places[i] = total_data_recved; // data will be placed at places[i] in buffer [i] for each rank
        total_data_recved += var_counts[i];
    }

    // Allocate memory for the receive buffer
    int* recvbuf = (int*)malloc(total_data_recved * sizeof(int));

    // Perform the gather operation
    for (int i = 0; i < size; i++) {
        if (i != rank) { // SEnding data if process' rank is not its own\
            // Sending data of variable size to each other slave + sending the size of dat that is sent each time
            MPI_Send(&variable_data_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(data_to_send, variable_data_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    // Recieving variable length data from each slave
    for (int i = 0; i < size; i++) {
        if (i != rank) { // checking if rank of process is not its own
            int var_recv_size;
            // Recieving variable data size from all other slaves 
            MPI_Recv(&var_recv_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // creating a temp buffer and allocating memory to it to store the varibale size data of length size recieved above
            int* temp_recvbuf = (int*)malloc(var_recv_size * sizeof(int));
            MPI_Recv(temp_recvbuf, var_recv_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // loop for placing data at respective index that we calculated above in places 
            for (int j = 0; j < var_recv_size; j++) {
                recvbuf[places[i] + j] = temp_recvbuf[j];
            }

            free(temp_recvbuf); // memory deallocation for temp buffer
        } else {
            // Copy the data from data_to_send to the appropriate location in the receive buffer
            for (int j = 0; j < variable_data_size; j++) {
                recvbuf[places[i] + j] = data_to_send[j];
            }
        }
    }
    // getting container id to show code is working on containers
    MPI_Get_processor_name(processor_name, &name_len);
    // Printing data recieved to each process
    printf("\n\t\t \x1B[35m==========\x1B[0m Process with Rank # \x1B[33m%d\x1B[0m, Container ID # \x1B[31m%s\x1B[0m \x1B[35m========== \x1B[0m\n", rank, processor_name);
    printf("Data recieved: [");
    for (int i = 0; i < total_data_recved; i++) {
        printf("\x1B[32m%d\x1B[0m ", recvbuf[i]); // printing data recieved by each container / process
    }
    printf("]\n");

    // Memory deallocation | Clean Up process
    free(data_to_send);
    free(recvbuf);
    free(var_counts);
    free(places);

    MPI_Finalize(); // Finalizing MPI Parallel Sequence
    return 0;
}
