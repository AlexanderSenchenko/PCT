#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

int main(int argc, char *argv[])
{
	int rank, commsize, prev, next;

	// for (int count = 1; count <= 1024 * 1024; count *= 1024) {
		// int count = 1;
		int count = 1024;
		// int count = 1024 * 1024;
		
		// char *msg = malloc(sizeof(char) * count);

		MPI_Init(&argc, &argv);

		char *outmsg = calloc(count, sizeof(char));
		char *inmsg = calloc(count, sizeof(char));

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &commsize);

		prev = (rank + commsize - 1) % commsize;
		next = (rank + 1) % commsize;

		double t = MPI_Wtime();
		for (int i = 0; i < commsize; i++) {

			// snprintf(msg, NELEMS(msg), "Rank %d of %d", rank, commsize);
			// MPI_Send(msg, NELEMS(msg), MPI_CHAR, next, 0, MPI_COMM_WORLD);

			// MPI_Send(msg, count, MPI_CHAR, next, 0, MPI_COMM_WORLD);

			MPI_Status status;

			// MPI_Recv(msg, count, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);

			MPI_Sendrecv(outmsg, count, MPI_CHAR, next, 0, 
				inmsg, count, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);

			// printf("Rank %d. Test_prev %d, '%s'\n", rank, status.MPI_SOURCE, msg);
		}
		t = MPI_Wtime() - t;
		// printf("t(rank: %d) = %f\n", rank, t);

		double t_glob = 0;
		MPI_Reduce(&t, &t_glob, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		if (rank == 0)
			printf("Time all prog: %f\n", t_glob);

		free(outmsg);
		free(inmsg);

		MPI_Finalize();
	// }
	return 0;
}
