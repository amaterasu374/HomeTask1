#include "matrix_16.h"
#include <stdio.h>
#include <math.h>

#define EPS 1e-14

    int col;
    int rows;
    int cols;
    double sum;
    double diff;
    double res2;
    double b2;
    double s;
    double exact;
    
double formula_f(int k, int n, int i, int j) {
    switch (k) {
        case 1: {
            int max = (i > j) ? i : j;
            return (double)(n - max + 1);
        }
        case 2: {
            int max = (i > j) ? i : j;
            return (double)max;
        }
        case 3:
            return fabs((double)(i - j));
        case 4:
            return 1.0 / (double)(i + j - 1);
        default:
            return 0.0;
    }
}

static int read_matrix_file(int n, double *A, const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "ERROR: can't open file '%s'\n", filename);
        return 1;
    }

    for (int i = 0; i < n * n; ++i) {
        if (fscanf(input, "%lf", &A[i])!= 1) {
            fprintf(stderr, "ERROR: incorrect file '%s'\n", filename);
            fclose(input);
            return 2;
        }
    }

    fclose(input);
    return 0;
}

int init_matrix(int n, int k, const char *filename, double *A) {
    if (k == 0) {
        if (filename == NULL) {
            fprintf(stderr, "ERROR: empty file\n");
            return 1;
        }
        return read_matrix_file(n, A, filename);
    }

    if (k < 1 || k > 4) {
        fprintf(stderr, "ERROR: incorrect k = %d\n", k);
        return 1;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            A[(i - 1) * n + (j - 1)] = formula_f(k, n, i, j);
        }
    }

    return 0;
}

void build_b(int n, const double *A, double *b) {
    int index = (n + 1) / 2;

    for (int i = 0; i < n; ++i) {
        sum = 0.0;

        for (int k = 0; k <= index; ++k) {
            col = 2 * k;
            if (col < n) {
                sum += A[i * n + col];
            }
        }

        b[i] = sum;
    }
}

void print_matrix(int l, int n, const double *M, int m) {
    rows = (l < m) ? l : m;
    cols = (n < m) ? n : m;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf(" %10.3e", M[i * n + j]);
        }
        printf("\n");
    }
}

double residual_norm_system(int n, const double *A, const double *x, const double *b) {
    res2 = 0.0;
    b2 = 0.0;

    for (int i = 0; i < n; ++i) {
        sum = 0.0;
        for (int j = 0; j < n; ++j) {
            sum += A[i * n + j] * x[j];
        }

        diff = sum - b[i];
        res2 += diff * diff;
    }

    for (int i = 0; i < n; ++i) {
        b2 += b[i] * b[i];
    }

    if (b2 < EPS) {
        return sqrt(res2);
    }

    return sqrt(res2) / sqrt(b2);
}

double error_norm(int n, const double *x) {
    s = 0.0;

    for (int i = 0; i < n; ++i) {
        exact = (i % 2 == 0) ? 1.0 : 0.0; // 1,0,1,0,...
        diff = x[i] - exact;
        s += diff * diff;
    }

    return sqrt(s);
}
