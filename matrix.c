#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "matrix.h"

double** generate_random_matrix(int dim) {
    printf("Generating matrix (%i x %i)...\n", dim, dim);
    double** result = create_matrix(dim);
    if (result == NULL) {
        return NULL;
    }

    struct timespec ts;
    clock_gettime(CLOCK, &ts);

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
    if (result == NULL) {
        printf("Failed to allocate result matrix\n");
        exit(0);
    }

    printf("Transposing right matrix...\n");
    double** transposed = transpose(matrix2, dim);
    if (transposed == NULL) {
        printf("Failed to allocate transpose matrix\n");
        exit(0);
    }

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

double** transpose(double** matrix, int dim) {
    double** result = create_matrix(dim);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

double** create_matrix(int dim) {
    double** result = malloc(sizeof(double*) * dim);
    if (result == NULL) {
        return NULL;
    }
    // initialize result matrix
    for (int i = 0; i < dim; ++i) {
        result[i] = malloc(sizeof(double) * dim);
        if (result[i] == NULL) {
            free_matrix(result, dim);
            return NULL;
        }
    }
    return result;
}

void free_matrix(double** matrix, int dim) {
    if (matrix != NULL) {
        for (int i = 0; i < dim; ++i) {
            free(matrix[i]);
        }
    }
    free(matrix);
}
