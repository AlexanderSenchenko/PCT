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

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

	char *sbuf = calloc(count, sizeof(char));
	// char sbuf[128];
	// snprintf(sbuf, NELEMS(sbuf), "Rank %d of %d", rank, commsize);

	double t = MPI_Wtime();

	if (rank != 0) {
		MPI_Send(sbuf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		// MPI_Send(sbuf, NELEMS(sbuf), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else {
		MPI_Status status;
		char *rbuf = calloc(count * commsize, sizeof(char));
		// char rbuf[128];
		for (int i = 1; i < commsize; i++) {
			MPI_Recv(rbuf + count * i, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
			// MPI_Recv(rbuf + NELEMS(rbuf) * i, NELEMS(rbuf), MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
		}

		MPI_Sendrecv(sbuf, count, MPI_CHAR, 0, 0,
			rbuf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

		// MPI_Sendrecv(sbuf, NELEMS(sbuf), MPI_CHAR, 0, 0,
		// 	rbuf, NELEMS(rbuf), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

	}

	t = MPI_Wtime() - t;
	printf("t(rank: %d) = %f\n", rank, t);

	double t_glob = 0;
	MPI_Reduce(&t, &t_glob, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
	printf("Time all prog(size %d): %f\n", count, t_glob);

	free(sbuf);

	MPI_Finalize();

	return 0;
}
