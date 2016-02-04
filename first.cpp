#include<mpi.h>
main(int argc, char *argv[])
{
	int npes, myrank;
	//printf ("%d, %s\n ", argc, argv[0]);
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	printf("From process %d out of %d, Hello World!\n", myrank, npes);
	MPI_Finalize();
}

