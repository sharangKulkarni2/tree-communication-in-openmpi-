#include<stdio.h>
#include<stdlib.h>
//#include<omp.h>
#include<mpi.h>
#define SIZE 8

double sum(double my_value, int my_rank, int p, MPI_Comm comm);

int main(){
	int size, rank;
	int global_sum;
	int x;
	
	MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    x = rank + 1;
    printf("%d %d\n",rank, x);
    MPI_Barrier(MPI_COMM_WORLD);
    global_sum = sum(x, rank, size, MPI_COMM_WORLD);

    if(rank == 0)
    	printf("%d\n",global_sum);

    MPI_Finalize();
    return 0;
}

double sum(double my_value, int my_rank, int p, MPI_Comm comm){

double sum = my_value;
double temp;
int partner;
int done = 0;
unsigned bitmask = (unsigned) 1;

while (!done && bitmask < p){
	partner = my_rank ^ bitmask;
	if (my_rank < partner) {
		if (partner < p) {
			MPI_Recv(&temp, 1, MPI_DOUBLE, partner, 0, comm, 
				MPI_STATUS_IGNORE);
			sum += temp;
		}
		bitmask <<= 1;
} 
else {
	MPI_Send(&sum, 1, MPI_DOUBLE, partner, 0, comm); 
	done = 1;
  }

}

return sum;

}