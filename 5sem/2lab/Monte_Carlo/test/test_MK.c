#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

const double PI = 3.14159265358979323846;
const int n = 1000000000;

double func(double x, double y)
{
	return 3 * pow(y, 2) * pow(sin(x), 2);
}

double wtime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 1E-6;
}

double getrand()
{
	return (double)rand() / RAND_MAX;
}

int main(int argc, char *argv[])
{
	srand(time(NULL)); 
	int in = 0, n0 = n;
	double sum_d = 0;
	double sum = 0;

	double t = wtime();

	for (int i = 0; i < n0; i++) {
		double x = getrand() * PI;
		double y = getrand();

		if (y <= sin(x)) {
			in++;
			sum += func(x, y);
		}

		sum_d += sin(x);
	}

	t = wtime() - t;

	double v1 = PI * in / n0;
	double res1 = v1 * sum / in;

	printf("v1 = %.6f\n", v1);
	printf("res1 = %.6f\n", res1);
	
	
	double v2 = PI * sum_d / n0;
	double res2 = v2 * sum / in;
	
	printf("v2 = %.6f\n", v2);
	printf("res2 = %.6f\n", res2);


	return 0;
}