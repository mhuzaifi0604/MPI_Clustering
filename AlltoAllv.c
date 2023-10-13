#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<time.h>
#include<unistd.h>

#define elem_ps (int[]){1,2}
#define indices_ps (int[]){0,1}


int main(int argc, char** argv){
    srand(time(NULL));
    MPI_Init(&argc, &argv);
    int rank, size, name_len, var_send, var_recv, indices_send, indices_recv;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int var_size = rank + 1;
    int *data_to_send = (int *)malloc(sizeof(int) * var_size);
    int *data_to_recv = (int *)malloc(sizeof(int) * var_size);

    printf ("Process # [%d] --> Size # [%d]\n", rank, var_size);
    MPI_Barrier(MPI_COMM_WORLD);

    for(int i = 0 ; i < var_size; i++){
        data_to_send[i] = (rank + 1) * 10 + i;
        data_to_recv[i] = data_to_send[i];
        printf ("Rank: [%d] --> Contains Data: [%d]\n", rank, data_to_send[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int total_data_recieved = 0;
    int *send_size = (int *)malloc(sizeof(int) * size);
    int *recv_indices = (int *)malloc(sizeof(int) * size);

    for(int i = 0; i < size; i++){
        send_size[i] = i++;
        recv_indices[i] = total_data_recieved;
        total_data_recieved += send_size[i];
    }
    if (rank == 0){
        var_send = elem_ps[0];
        indices_send = indices_ps[0];
        var_recv = elem_ps[1];
        indices_recv = indices_ps[0];
        var_size += 1;
    }else if (rank == 1){
        var_send = elem_ps[0];
        indices_send = indices_ps[1];
        var_recv = elem_ps[1];
        indices_recv = indices_ps[1];
        var_size += 1;
    }
    else if (rank == 2){
        var_send = elem_ps[1];
        indices_send = indices_ps[0];
        var_recv = elem_ps[0];
        indices_recv = indices_ps[0];
        var_size -= 1;
    }else if(rank == 3){
        var_send = elem_ps[1];
        indices_send = indices_ps[1];
        var_recv = elem_ps[0];
        indices_recv = elem_ps[1];
        var_size -= 1;
    }
    printf("\n\n");
    if (rank == 0){
        MPI_Send(&data_to_send[0], 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[0], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data_to_recv[1], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Master recived [%d] and [%d]\n", data_to_recv[0], data_to_recv[1]);
        printf("process 0: ");
        for (int i = 0; i < var_size; i++){
            printf ("[%d]", data_to_recv[i]);
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
        // printf("Master recived [%d] and [%d]\n", data_to_recv[0], data_to_recv[1]);
    }else if (rank == 1){
        MPI_Send(&data_to_send[1], 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[1], 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data_to_recv[2], 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 recived [%d] and [%d] from process 3.\n", data_to_recv[1], data_to_recv[2]);
        printf("process 1: ");
        for (int i = 0; i < var_size; i++){
            printf ("[%d]", data_to_recv[i]);
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
        // printf("Process 1 recived [%d] and [%d] from process 3.\n", data_to_recv[0], data_to_recv[1]);
    }
    else if(rank == 2){
        MPI_Send(&data_to_send[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&data_to_send[1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 2 recieved [%d] from Master.\n", data_to_recv[0]);
        printf("process 2: ");
        for (int i = 0; i < var_size; i++){
            printf ("[%d]", data_to_recv[i]);
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
        // printf("Process 2 recieved [%d] from Master.\n", data_to_recv[0]);
    }else if(rank == 3){
        MPI_Send(&data_to_send[2], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&data_to_send[3], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_to_recv[2], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Last slave Recieved: [%d]\n from Process 1\n", data_to_recv[0]);
        printf("process 3: ");
        for (int i = 0; i < var_size; i++){
            printf ("[%d]", data_to_recv[i]);
        }
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    free(data_to_recv);
    free(data_to_send);
    free(send_size);
    free(recv_indices);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}