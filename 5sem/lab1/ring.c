#include <stdio.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) /  sizeof ((x)[0]))

int main(int argc, char *argv[])
{
    int rank, num_proz;
    char msg[128];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proz);

    for (int i = 0; i < num_proz - 1; i++) {

        snprintf(msg, NELEMS(msg), "Rank %d of %d", rank, num_proz);

        if (rank == num_proz) {
            MPI_Send(msg, NELEMS(msg), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(msg, NELEMS(msg), MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
        }

        MPI_Status status;
        if (rank == 0) {
            MPI_Recv(msg, NELEMS(msg), MPI_CHAR, num_proz - 1, 0, MPI_COMM_WORLD, &status);
        } else {
            MPI_Recv(msg, NELEMS(msg), MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &status);
        }

        printf("Rank %d. Test %d, '%s'\n", rank, status.MPI_SOURCE, msg);
    }

    MPI_Finalize();
    return 0;
}
