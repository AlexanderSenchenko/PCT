#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int rank, commsize;

	// int count = 1024;
	int count = 1024 * 1024;
	char *rbuf;
	double t, t_glob;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

	// for (int i = 0; i < 2; i++) {
		rbuf = calloc(count, sizeof(char));
		// char rbuf[128];

		t = MPI_Wtime();
		
		if (rank == 0) {
			char *sbuf = calloc(count, sizeof(char));
			// char sbuf[128];
			// snprintf(sbuf, NELEMS(sbuf), "Rank %d of %d", rank, commsize);

			for (int i = 1; i < commsize; i++) {
				MPI_Send(sbuf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
				// MPI_Send(sbuf, NELEMS(sbuf), MPI_CHAR, i, 0, MPI_COMM_WORLD);
			}

			MPI_Status status;
			// MPI_Sendrecv(sbuf, NELEMS(sbuf), MPI_CHAR, 0, 0,
			// 	rbuf, NELEMS(rbuf), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Sendrecv(sbuf, count, MPI_CHAR, 0, 0,
				rbuf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		} else {
			MPI_Status status;
			// MPI_Recv(rbuf, NELEMS(rbuf), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(rbuf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		}

		// printf("Rank %d. Test_send %d, '%s'\n", rank, status.MPI_SOURCE, rbuf);

		t = MPI_Wtime() - t;
		printf("t(rank: %d) = %f\n", rank, t);

		t_glob = 0;
		MPI_Reduce(&t, &t_glob, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			printf("Time all prog, size %d: %f\n", count, t_glob);
		}

		free(rbuf);
		// count *= 1024;

		// MPI_Barrier(MPI_COMM_WORLD);
	// }

	MPI_Finalize();

	return 0;
}
