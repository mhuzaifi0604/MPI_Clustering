/*
Muhammad Huzaifa
20i-0604
Assignment # 02
Task # 01 part c All To All implementation using mpi send and mpi recieve
*/

// including necessory libraries
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<time.h>
#include<unistd.h>

// defining constant global arrays for later use
#define elem_ps (int[]){1,2}
#define indices_ps (int[]){0,1}


int main(int argc, char** argv){
    srand(time(NULL));
    // Initializing MPI_Sequence
    MPI_Init(&argc, &argv);
    // Declaring variables for later use for each container
    int rank, size, name_len, var_send, var_recv, indices_send, indices_recv;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);// Retreiving rank of process container b/w parallel processing
    MPI_Comm_size(MPI_COMM_WORLD, &size);// Retreiving number of processes container b/w parallel processing
    
    // initializing variable size for each process
    int var_size = rank + 1;
    // initializing arrays to send and recieve variable data b/w processes
    int *data_to_send = (int *)malloc(sizeof(int) * var_size);
    int *data_to_recv = (int *)malloc(sizeof(int) * var_size);

    // getting container IDS to show code is working on cluster
    MPI_Get_processor_name(processor_name, &name_len);

    // printing sizes of process 
    printf ("\x1B[33mProcess # [%d], Container ID:[\x1B[31m %s \x1B[0m] --> Size # [%d]\x1B[0m\n", rank,processor_name, var_size);
    MPI_Barrier(MPI_COMM_WORLD); // waiting for printing sizes of all processes

    // Initializing processes with random numbers at each sized index
    for(int i = 0 ; i < var_size; i++){
        data_to_send[i] = (((rand() % 10 ) + 1) * (rank + 1) )+ i;
        data_to_recv[i] = data_to_send[i];
        // printing element at each index of array
        printf ("\x1B[36mRank: [%d] --> Contains Data: [%d]\x1B[0m\n", rank, data_to_send[i]);
    }
    // waiting for prev completion
    MPI_Barrier(MPI_COMM_WORLD);
    int total_data_recieved = 0;
    // initializing dynamic arrays to send and recieve total elements and index for sending and recieving
    int *send_size = (int *)malloc(sizeof(int) * size);
    int *recv_indices = (int *)malloc(sizeof(int) * size);

    // setting recieving indices for each process and incrementing sending index for next index
    for(int i = 0; i < size; i++){
        send_size[i] = i++;
        recv_indices[i] = total_data_recieved;
        total_data_recieved += send_size[i];
    }
    // Master Process
    if (rank == 0){
        var_send = elem_ps[0]; // sending its single element
        indices_send = indices_ps[0]; // sending from index defined in global array
        var_recv = elem_ps[1]; // defining elements to be recieved by master
        indices_recv = indices_ps[0]; // defining where to store elements in master
        var_size += 1; // increasing size on recieval
    }else if (rank == 1){ // Process # 01
        var_send = elem_ps[0]; // sending single element
        indices_send = indices_ps[1]; // sending from index specified globally
        var_recv = elem_ps[1]; // defining no of elements to be recieved by process 01
        indices_recv = indices_ps[1]; // defining index to store recieving elements
        var_size += 1; // increasing size on recieval
    }
    else if (rank == 2){ // 2nd last slave
        var_send = elem_ps[1]; // sending 2 elements to a process
        indices_send = indices_ps[0]; // defining index to pic elements from
        var_recv = elem_ps[0]; // defining elements to recieve
        indices_recv = indices_ps[0]; // defining index to store recieved element
        var_size -= 1;// decreasing size on recieval
    }else if(rank == 3){ // Last Slave
        var_send = elem_ps[1]; // sending 2 elements
        indices_send = indices_ps[1]; // defining index to pick element from
        var_recv = elem_ps[0]; // defining elements to recieve
        indices_recv = elem_ps[1]; // definign index to store recieved element
        var_size -= 1;// decreasing size on recieval
    }
    printf("\n\n");
    if (rank == 0){ // Master Process
        MPI_Send(&data_to_send[0], 1, MPI_INT, 2, 0, MPI_COMM_WORLD); // Sending its single element
        // Reciving arriving elements from other process
        MPI_Recv(&data_to_recv[0], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data_to_recv[1], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Printing recieved elements from other process
        printf("\x1B[35mMaster recived [%d] and [%d]\x1B[0m\n", data_to_recv[0], data_to_recv[1]);
        printf("\x1B[31mprocess 0\x1B[0m: ");
        for (int i = 0; i < var_size; i++){
            printf ("\x1B[31m[%d]\x1B[0m, ", data_to_recv[i]); // printing all elements of master
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }else if (rank == 1){ // 1st slave
        MPI_Send(&data_to_send[1], 1, MPI_INT, 3, 0, MPI_COMM_WORLD);// Sending its single element
        // Reciving arriving elements from other process
        MPI_Recv(&data_to_recv[1], 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data_to_recv[2], 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Printing recieved elements from other process
        printf("\x1B[35mProcess 1 recived [%d] and [%d] from process 3.\x1B[0m\n", data_to_recv[1], data_to_recv[2]);
        printf("\x1B[32mprocess 1\x1B[0m: ");
        for (int i = 0; i < var_size; i++){
            printf ("\x1B[32m[%d]\x1B[0m, ", data_to_recv[i]); // printing all elements of slave # 01
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }
    else if(rank == 2){ // Slave # 02
        // / Sending its elements to other process
        MPI_Send(&data_to_send[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&data_to_send[1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Reciving its single element
        // Printing recieved elements from other process
        printf("\x1B[35mProcess 2 recieved [%d] from Master.\x1B[0m\n", data_to_recv[0]);
        printf("\x1B[33mprocess 2\x1B[0m: ");
        for (int i = 0; i < var_size; i++){
            printf ("\x1B[33m[%d]\x1B[0m, ", data_to_recv[i]); // printing all elements of 2nd slave
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }else if(rank == 3){ // Last slave
        // sending its elements to other process
        MPI_Send(&data_to_send[2], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&data_to_send[3], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[2], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // reciving single element from other process
        // Printing data recieved by other process
        printf("\x1B[35mLast slave Recieved: [%d] from Process 1\x1B[0m\n", data_to_recv[0]);
        printf("\x1B[34mprocess 3\x1B[0m: ");
        for (int i = 0; i < var_size; i++){
            printf ("\x1B[34m[%d]\x1B[0m, ", data_to_recv[i]); //printing elements of last slave
        }
        printf("\n");
        // Waiting every process to be done with every thing before here
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Deallocating allocated dynamic memory
    free(data_to_recv);
    free(data_to_send);
    free(send_size);
    free(recv_indices);
    MPI_Barrier(MPI_COMM_WORLD);
    // Finalizing mpi sequence
    MPI_Finalize();
    return 0;
}