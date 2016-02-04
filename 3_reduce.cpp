#include<iostream>
#include<mpi.h>
using namespace std;

#define ARR_SIZE 4096

/* Broadcast version:
 * Proc0 broadcasts the array to every other proc,
 * receives the part sums, and then sums up and returns
 */
int main(int argc, char **argv)
{
	int num_procs, myrank;
	double start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	int part_size = int(ARR_SIZE / num_procs);

	/* Initialize the array */
	int arr[ARR_SIZE];

	if(0 == myrank) {
		for(int i=0; i<ARR_SIZE; ++i) {
			arr[i] = i+1;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();

	/* Broadcast to each process */
	MPI_Bcast(arr, ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	/* sum up your own part */
	long long local_sum = 0, global_sum = 0;

	for(int i=myrank*part_size; i<(myrank+1)*part_size; ++i) {
		local_sum += arr[i];
	}

	MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myrank == 0) {
		printf("The final sum is %d\n", global_sum);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	end = MPI_Wtime();

	if (myrank==0)
		printf("Time taken: %fs\n", end-start);

	MPI_Finalize();
	return 0;
}
