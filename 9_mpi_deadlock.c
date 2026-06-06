#include <stdio.h>
#include "mpi.h"

#define MSG_SIZE 1000000 // large message
int main(int argc, char* argv[])
{
    int rank, size;
    int send_buf[MSG_SIZE], recv_buf[MSG_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);               // initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get this process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // get total number of processes

    // fill send buffer with this process's rank
    int i;
    for (i = 0; i < MSG_SIZE; i++) send_buf[i] = rank;

    if (rank == 0)
    {
        // process 0 tries to send first, blocks here waiting for process 1 to receive
        // but process 1 is also stuck trying to send -> deadlock
        MPI_Send(send_buf, MSG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(recv_buf, MSG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process 0 received first element: %d\n", recv_buf[0]); // never reached
    }
    else if (rank == 1)
    {
        // process 1 also tries to send first, blocks waiting for process 0 to receive
        // process 0 is not receiving yet, it is also stuck in MPI_Send
        MPI_Send(send_buf, MSG_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(recv_buf, MSG_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process 1 received first element: %d\n", recv_buf[0]); // never reached
    }

    MPI_Finalize();
    return 0;
}
