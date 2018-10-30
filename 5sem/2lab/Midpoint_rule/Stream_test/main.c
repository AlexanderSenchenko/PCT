#include <stdio.h>
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

int main()
{
    const double a = 0.1, b = 1;

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
    printf("Res integr(test): %.6f\n", sq[k]);
    printf("Time(test) %.6f\n", t);

    return 0;
}
