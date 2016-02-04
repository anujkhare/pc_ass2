#include<iostream>
#include<string>
#include<vector>
#include<mpi.h>
using namespace std;

#define ARR_SIZE 4096

/* Naive version:
 * Proc0 sends parts of the array to every other proc,
 * receives the parts, and then sums up and returns
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
		/* Sum up your own part, then add the sums with received sums */
		long long sum = 0;

		/* sum up your own part */
		for(int i=0; i<part_size; ++i) {
			sum += arr[i];
		}

		/* Send apt part of the array to p */
		for(int p=1; p<num_procs; ++p) {
			MPI_Send(arr + p*part_size, part_size, MPI_INT, p, 0, MPI_COMM_WORLD);
			printf("Send array part to %d\n", p);
		}

		/* Receive part sum from p */
		for(int p=1; p<num_procs; ++p) {
			long long rec;
			MPI_Recv(&rec, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += rec;
		}

		printf("The final sum is %d\n", sum);

	} else {
		/* Receive part of the array */
		int arr_part[part_size], sum=0;
		MPI_Recv(&arr_part, part_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("received by proc %d: %d\n", myrank, arr_part[0]);
		/* sum and send the sum back */
		for (int i=0; i<part_size; ++i) {
			sum += arr_part[i];
		}
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
