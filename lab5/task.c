#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

const int threshold = 1000;

void swap(int *vi, int *vj)
{
	int buf = *vi;
	*vi = *vj;
	*vj = buf;
}

void partition(int *v, int *i, int *j, int low, int high) {
	*i = low;
	*j = high;
	int pivot = v[(low + high) / 2];
	do {
		while (v[*i] < pivot) (*i)++;
		while (v[*j] > pivot) (*j)--;
		if (*i <= *j) {
			swap(&v[*i], &v[*j]);
			(*i)++;
			(*j)--;
		}
	} while ((*i) <= (*j));
}

void quicksort_tasks(int *v, int low, int high) {
	int i, j;
	partition(v, &i, &j, low, high);
	if (high - low < threshold || (j - low < threshold || high - i < threshold)) {
		if (low < j)
			quicksort_tasks(v, low, j);
		if(i < high)
			quicksort_tasks(v, i, high);
		} else {
			#pragma omp task untied
			{
				quicksort_tasks(v, low, j);
			}
		quicksort_tasks(v, i, high);
	}
}

int main(int argc, char const *argv[])
{
	omp_set_nested(1);

	int n = 9;
	int v[9] = {3, 1, 4, 5, 9, 2, 6, 8, 7};

	#if 0
	for (int i = 0; i < 9; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	#endif

	#pragma omp parallel
	{
		#pragma omp single
		quicksort_tasks(v, 0, n - 1);
	}

	#if 1
	for (int i = 0; i < 9; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	#endif

	return 0;
}