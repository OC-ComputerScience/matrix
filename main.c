#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "matrix.h"

double ts_to_ms(struct timespec* ts) {
    return (((double) ts->tv_sec) * 1000.0) + (((double) ts->tv_nsec) / 1000000.0);
}

void do_calculation(double** matrix1, double** matrix2, int dim, int single_threaded) {
    if (single_threaded) {
        printf("Performing single-threaded calculation...\n");
    } else {
        printf("Performing multi-threaded calculation...\n");
    }

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK, &start);
    do_multiply(matrix1, matrix2, dim, single_threaded);
    clock_gettime(CLOCK, &end);

    double start_time = ts_to_ms(&start);
    double end_time = ts_to_ms(&end);

    double time = end_time - start_time;

    if (single_threaded) {
        printf("Single-threaded execution time: %f ms\n", time);
    } else {
        printf("Multi-threaded execution time: %f ms\n", time);
    }
}

int main(int arg_c, const char** arg_v) {
    int dim;
    if (arg_c != 2) {
        printf("No size given, using 1000 as default\n");
        dim = 1000;
    } else {
        dim = (int) strtol(arg_v[1], NULL, 10);
    }

    double** matrix1 = generate_random_matrix(dim);
    if (matrix1 == NULL) {
        printf("Failed to allocate matrix\n");
        return 0;
    }
    double** matrix2 = generate_random_matrix(dim);
    if (matrix2 == NULL) {
        printf("Failed to allocate matrix\n");
        return 0;
    }

    printf("\n");

    // single-threaded
    do_calculation(matrix1, matrix2, dim, 0);

    printf("\n");

    // multi-threaded
    do_calculation(matrix1, matrix2, dim, 1);

    free_matrix(matrix1, dim);
    free_matrix(matrix2, dim);

    return 0;
}
