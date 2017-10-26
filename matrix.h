#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#define NUM_MAX 1000.0

#ifdef __CYGWIN__
#define CLOCK CLOCK_MONOTONIC
#else
#define CLOCK CLOCK_MONOTONIC_RAW
#endif

/**
 * Generate a new random matrix of the given square dimension
 * @param dim The square dimension to generate the new matrix
 * @return The newly generated matrix, filled with random values between 0 and NUM_MAX
 */
double** generate_random_matrix(int dim);

/**
 * Multiply the two given matrices. If single_threaded is true, only use one thread.
 * @param matrix1 The left matrix to multiply
 * @param matrix2 The right matrix to multiply
 * @param dim The (square) dimension of both matrices
 * @param single_threaded Whether or not to do the calculation on a single thread
 */
void do_multiply(double** matrix1, double** matrix2, int dim, int single_threaded);

/**
 * Given two pointers to unallocated matrices, generate the two matrices based on the given dimension.
 * @param matrix1 Pointer to the first unallocated matrix
 * @param matrix2 Pointer to the second unallocated matrix
 * @param dim The (square) dimension to generate the matrices
 */
void get_matrices(double ***matrix1, double ***matrix2, int dim);

/**
 * Return a transpose of the given matrix
 * @param matrix The matrix to transpose
 * @param dim The dimension of the given matrix
 * @return The transpose of the given matrix
 */
double** transpose(double** matrix, int dim);

/**
 * Allocate a new matrix of size dim x dim
 * @param dim The (square) dimension of the matrix to allocate. The matrix will be uninitialized.
 * @return The root pointer to the newly allocated matrix.
 */
double** create_matrix(int dim);

/**
 * Free the dynamically allocated matrix, including every row
 * @param matrix The root pointer to the matrix to free
 * @param dim The dimension of the matrix
 */
void free_matrix(double** matrix, int dim);

#endif //MATRIX_MATRIX_H
