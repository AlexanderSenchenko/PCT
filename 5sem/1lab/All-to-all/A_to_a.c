#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int rank, commsize;

	int count = 1024;
	// int count = 1024 * 1024;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

	char *sbuf = calloc(count, sizeof(char));
	// char sbuf[128];
	// snprintf(sbuf, NELEMS(sbuf), "Rank %d of %d", rank, commsize);

	char *rbuf = calloc(count, sizeof(char));

	MPI_Request request[commsize * 2];
	// MPI_Status status[commsize];

	double t = MPI_Wtime();

	for (int i = 0; i < commsize; i++) {
		MPI_Isend(sbuf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i]);
	}

	MPI_Status status;
	for (int i = 0; i < commsize; i++) {
		// MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		MPI_Irecv(rbuf, count, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request[i + commsize]);
		printf("%d <- %d\n", rank, status.MPI_SOURCE);
	}

	t = MPI_Wtime() - t;
	printf("t(rank: %d) = %f\n", rank, t);

	MPI_Waitall(commsize * 2, request, MPI_STATUS_IGNORE);

	double t_glob = 0;
	MPI_Reduce(&t, &t_glob, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Time all prog: %f\n", t_glob);

	free(sbuf);

	MPI_Finalize();

	return 0;
}
