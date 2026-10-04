#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix_16.h"
#include "jordan.h"

int main(int argc, char **argv) {
    int n;
    int m;
    int k;
    int *arr = NULL;
    double t;
    double residual;
    double error;
    double *A = NULL;
    double *b = NULL;
    double *x = NULL;
    const char *filename = NULL;

    if (argc < 4) {
        fprintf(stderr, "ERROR: incorrect argc \n");
        return 1;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);
    k = atoi(argv[3]);

    if (k == 0) {
        if (argc != 5) {
            fprintf(stderr, "ERROR: incorrect file argc\n");
            return 1;
        }
        filename = argv[4];
    } else {
        if (argc != 4) {
            fprintf(stderr, "ERROR: incorrect formula argc\n");
            return 1;
        }

        if (k < 1 || k > 4) {
            fprintf(stderr, "ERROR: incorrect k\n");
            return 1;
        }
    }

    if (n <= 0 || m <= 0 || m > n) {
        fprintf(stderr, "ERROR: incorrect n or m\n");
        return 1;
    }

    A = (double *)malloc((size_t)n * n * sizeof(double));
    b = (double *)malloc((size_t)n * sizeof(double));
    x = (double *)malloc((size_t)n * sizeof(double));
    arr = (int *)malloc((size_t)n * sizeof(int));

    if (!A || !b || !x || !arr) {
        fprintf(stderr, "ERROR\n");
        free(A);
        free(b);
        free(x);
        free(arr);
        return 1;
    }

    if (init_matrix(n, k, filename, A) != 0) {
        free(A);
        free(b);
        free(x);
        free(arr);
        return 1;
    }

    build_b(n, A, b);

    printf("Matrix:\n");
    print_matrix(n, n, A, m);

    clock_t start = clock();
    int solve_res = jordan_solve(n, A, b, x, arr);
    clock_t end = clock();

    if (solve_res != 0) {
        fprintf(stderr, "ERROR: matrix\n");
        free(A);
        free(b);
        free(x);
        free(arr);
        return 1;
    }

    printf("Solution x:\n");
    print_matrix(1, n, x, m);

    t = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f sec\n", t);

    if (init_matrix(n, k, filename, A) != 0) {
        free(A);
        free(b);
        free(x);
        free(arr);
        return 1;
    }
    build_b(n, A, b);

    residual = residual_norm_system(n, A, x, b);
    printf("Residual norm: %e\n", residual);

    error = error_norm(n, x);
    printf("Error norm: %e\n", error);

    free(A);
    free(b);
    free(x);
    free(arr);

    return 0;
}
