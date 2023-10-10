#include <mpi.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char **argv){
    int rank, size, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data_to_send = rank;
    int *data_to_recv = (int*)malloc(size*sizeof(int));
    for(int i = 0 ; i < size; i++){
        if(i != rank){
            MPI_Send(&data_to_send, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Recv(&data_to_recv[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else{
            data_to_recv[i] = data_to_send;
        }
    }
    MPI_Get_processor_name(processor_name, &name_len);
    // Printing data recieved to each process
    printf("\n\t\t \x1B[35m==========\x1B[0m Process with Rank # \x1B[33m%d\x1B[0m, Container ID # \x1B[31m%s\x1B[0m \x1B[35m========== \x1B[0m\n", rank, processor_name);
    printf("Data recieved: [");
    for(int i = 0 ; i < size; i++){
        printf("\x1B[32m%d\x1B[0m ", data_to_recv[i]);
    }
    printf("]\n");
    free(data_to_recv);
    MPI_Finalize();
    return 0;

}