#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <sys/time.h>

const double eps = 1E-6;
const int n0 = 10000000;

double func(double x)
{
	return ((log(1 + x)) / x);
}

double wtime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 1E-6;
}

double stream(double a, double b)
{
	int n = n0, k;
	double sq[2], delta = 1;

	double t = wtime();

	for (k = 0; delta > eps; n *= 2, k ^= 1) {

		double h = (b - a) / n;

		double s = 0.0;
		for (int i = 0; i < n; i++)
			s += func(a + h * (i + 0.5));

		sq[k] = s * h;
		if (n > n0)
			delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;

	}

	t = wtime() - t;
    // printf("Res integr(test): %.6f\n", sq[k]);
    // printf("Time(test) %.6f\n", t);

	return t;
}

int main(int argc, char *argv[])
{

	const double a = 0.1, b = 1;
	int rank, commsize;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = n0, k;
	double sq[2], delta = 1;

	double t = MPI_Wtime();

	for (k = 0; delta > eps; n *= 2, k ^= 1) {

		int iter = n / commsize;
		int lb = rank * iter;
		int ub = (rank == commsize - 1) ? (n - 1) : (lb + iter - 1);

		double h = (b - a) / n;

		double lsum = 0.0;
		for (int i = lb; i <= ub; i++)
			lsum += func(a + h * (i + 0.5));

		MPI_Allreduce(&lsum, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		sq[k] *= h;
		if (n > n0)
			delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;

	}

	t = MPI_Wtime() - t;

	// double gsum = 0.0;
	// MPI_Reduce(&lsum, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	double t_max = 0.0;
	MPI_Reduce(&t, &t_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		// gsum *= h;
		// printf("Res integr: %.6f\n", sq[k]);
		printf("Time: %.6f\n", t_max);

        double t_stream = stream(a, b);

		printf("Time(strem): %.6f\n", t_stream);
		// printf("Time(strem): %.6f\n", t_stream);
		printf("%.6f\n", t_stream / t_max);
	}

	MPI_Finalize();

	return 0;
}
