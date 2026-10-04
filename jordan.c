#include "jordan.h"
#include <math.h>

#define EPS 1e-14

int jordan_solve(int n, double *A, double *b, double *x, int *arr) {
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }

    for (int s = 0; s < n; ++s) {
        int p = s;
        double max = fabs(A[s * n + s]);

        for (int j = s + 1; j < n; ++j) {
            double v = fabs(A[s * n + j]);
            if (v > max) {
                max = v;
                p = j;
            }
        }

        if (max < EPS) {
            return 1;
        }

        if (p != s) {
            for (int i = 0; i < n; ++i) {
                double tmp = A[i * n + s];
                A[i * n + s] = A[i * n + p];
                A[i * n + p] = tmp;
            }

            int tmp = arr[s];
            arr[s] = arr[p];
            arr[p] = tmp;
        }

        double element = A[s * n + s];

        for (int j = s; j < n; ++j) {
            A[s * n + j] /= element;
        }
        b[s] /= element;

        for (int i = 0; i < n; ++i) {
            if (i == s) continue;

            double factor = A[i * n + s];
            for (int j = s; j < n; ++j) {
                A[i * n + j] -= factor * A[s * n + j];
            }
            b[i] -= factor * b[s];
        }
    }

    for (int c = 0; c < n; ++c) {
        x[arr[c]] = b[c];
    }

    return 0;
}
