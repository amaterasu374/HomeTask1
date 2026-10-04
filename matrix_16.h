
int init_matrix(int n, int k, const char *filename, double *A);
double formula_f(int k, int n, int i, int j);
void build_b(int n, const double *A, double *b);
void print_matrix(int l, int n, const double *M, int m);
double residual_norm_system(int n, const double *A, const double *x, const double *b);
double error_norm(int n, const double *x);

