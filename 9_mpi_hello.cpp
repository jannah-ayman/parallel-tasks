#include <iostream>
#include "mpi.h"

int main(int argc, char* argv[])
{
	int size, rank;

	MPI_Init(&argc, &argv);            // initialize the MPI execution environment
	MPI_Comm_size(MPI_COMM_WORLD, &size); // get total number of processes
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get this process's ID (rank)

	// each process prints its own rank and the total process count
	printf("hello world from process %d of %d processes\n", rank, size);

	MPI_Finalize(); // clean up and exit MPI environment
}
