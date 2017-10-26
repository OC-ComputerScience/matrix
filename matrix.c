#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matrix.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

double** generate_random_matrix(int dim) {
    printf("Generating matrix (%i x %i)...\n", dim, dim);
    double** result = create_matrix(dim);

    struct timespec ts;

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif

    srandom((unsigned int) (ts.tv_nsec ^ ts.tv_sec));

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            result[i][j] = (((double) random()) / ((double) RAND_MAX)) * NUM_MAX;
        }
    }

    printf("Completed generation\n");
    return result;
}

void do_multiply(double** matrix1, double** matrix2, int dim, int single_threaded) {
    double** result = create_matrix(dim);

    printf("Transposing right matrix...\n");
    double** transposed = transpose(matrix2, dim);

    printf("Multiplying matrices...\n");

    if (single_threaded) {
        omp_set_num_threads(1);
    }

    #pragma omp parallel
    {
        if (omp_get_thread_num() == 0) {
            printf("Using %i thread(s)\n", omp_get_num_threads());
        }

        #pragma omp for
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                double dot_product = 0;
                for (int k = 0; k < dim; ++k) {
                    dot_product += matrix1[i][k] * transposed[j][k];
                }
                result[i][j] = dot_product;
            }
        }
    }

    free_matrix(transposed, dim);
    // We're not doing anything with the output, just timing the operation
    free_matrix(result, dim);
}

void get_matrices(double ***matrix1, double ***matrix2, int dim) {
    *matrix1 = generate_random_matrix(dim);
    *matrix2 = generate_random_matrix(dim);
}

double** transpose(double** matrix, int dim) {
    double** result = create_matrix(dim);
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

double** create_matrix(int dim) {
    double** result = malloc(sizeof(double*) * dim);
    // initialize result matrix
    for (int i = 0; i < dim; ++i) {
        result[i] = malloc(sizeof(double) * dim);
    }
    return result;
}

void free_matrix(double** matrix, int dim) {
    for (int i = 0; i < dim; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
