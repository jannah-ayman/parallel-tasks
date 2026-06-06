#include <stdio.h>
#include "mpi.h"

#define MSG_SIZE 1000000

int main(int argc, char* argv[])
{
    int rank, size;
    static int send_buf[MSG_SIZE], recv_buf[MSG_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);               // initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get this process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // get total number of processes

    // fill send buffer with this process's rank
    int i;
    for (i = 0; i < MSG_SIZE; i++) send_buf[i] = rank;

    if (rank == 0)
    {
        // send and receive simultaneously -- no blocking, no deadlock
        MPI_Sendrecv(
            send_buf, MSG_SIZE, MPI_INT, 1, 0,   // send to process 1
            recv_buf, MSG_SIZE, MPI_INT, 1, 0,   // receive from process 1
            MPI_COMM_WORLD, &status
        );
        printf("Process 0 sent %d, received %d\n", send_buf[0], recv_buf[0]);
    }
    else if (rank == 1)
    {
        MPI_Sendrecv(
            send_buf, MSG_SIZE, MPI_INT, 0, 0,   // send to process 0
            recv_buf, MSG_SIZE, MPI_INT, 0, 0,   // receive from process 0
            MPI_COMM_WORLD, &status
        );
        printf("Process 1 sent %d, received %d\n", send_buf[0], recv_buf[0]);
    }

    MPI_Finalize();
    return 0;
}
