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


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	int part_size = ARR_SIZE / num_procs;

	/* Initialize the array */
	int arr[ARR_SIZE];
	//printf("Hey there %d\n", myrank);

	if(0 == myrank) {
		for(int i=0; i<ARR_SIZE; ++i) {
			arr[i] = i+1;
		}
	}

	if (0 == myrank) {
		printf("0000 %d %d\n", arr[0], arr[ARR_SIZE-1]);
		/* Broadcast to each process */
		MPI_Bcast(arr, ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Broadcast array\n");
	} else {
		printf("Received %d %d\n", arr[0], arr[ARR_SIZE-1]);
	}

	long long sum = 0;

	/* sum up your own part */
	for(int i=myrank*part_size; i<(myrank+1)*part_size; ++i) {
		sum += arr[i];
	}

	if (0 != myrank) {
		/* send the sum back */
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	} else {
		/* receive from others and sum up */
		long long temp = 0;
		for (int p=1; p<num_procs; ++p) {
			MPI_Recv(&temp, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += temp;
		}
		printf("The final sum is %d\n", sum);
	}


	MPI_Finalize();
	return 0;
}
