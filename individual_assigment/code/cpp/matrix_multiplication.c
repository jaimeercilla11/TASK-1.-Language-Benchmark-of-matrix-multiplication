#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>

typedef struct {
    int size;
    double execution_time;
    double memory_used;
} BenchmarkResult;

double get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#ifdef __APPLE__
    return usage.ru_maxrss / (1024.0 * 1024.0);
#else
    return usage.ru_maxrss / 1024.0;
#endif
}

double** create_matrix(int rows, int cols, double value) {
    double** matrix = (double**) malloc(rows * sizeof(double*));
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*) malloc(cols * sizeof(double));
        if (!matrix[i]) return NULL;
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value;
        }
    }
    return matrix;
}

void free_matrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

double** matrix_multiply(double** A, double** B, int n, int m, int p) {
    double** C = (double**) malloc(n * sizeof(double*));
    if (!C) return NULL;
    for (int i = 0; i < n; i++) {
        C[i] = (double*) calloc(m, sizeof(double));
        if (!C[i]) return NULL;
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

BenchmarkResult benchmark_matrix_multiplication(int size) {
    BenchmarkResult result;
    result.size = size;
    double** A = create_matrix(size, size, 1.5);
    double** B = create_matrix(size, size, 2.5);
    double mem_before = get_memory_usage();
    clock_t start = clock();
    double** C = matrix_multiply(A, B, size, size, size);
    clock_t end = clock();
    double mem_after = get_memory_usage();
    result.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    result.memory_used = mem_after - mem_before;
    free_matrix(A, size);
    free_matrix(B, size);
    free_matrix(C, size);
    return result;
}

void run_benchmarks(int sizes[], int num_sizes) {
    printf("%-10s %-15s %-15s\n", "Size", "Time (s)", "Memory (MB)");
    printf("---------------------------------------------------------------------\n");
    double total_time = 0.0, total_memory = 0.0;
    int successful_runs = 0;
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        BenchmarkResult r = benchmark_matrix_multiplication(size);
        total_time += r.execution_time;
        total_memory += r.memory_used;
        successful_runs++;
        printf("%-10d %-15.4f %-15.2f\n", size, r.execution_time, r.memory_used);
    }
    if (successful_runs > 0) {
        printf("\nSUMMARY\n");
        printf("=====================================================================\n");
        printf("Total execution time: %.2f seconds\n", total_time);
        printf("Average memory usage: %.2f MB\n", total_memory / successful_runs);
        printf("Largest matrix tested: %dx%d\n", sizes[successful_runs - 1], sizes[successful_runs - 1]);
    }
}

int main() {
    int test_sizes[] = {64, 128, 256, 512};
    int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);
    run_benchmarks(test_sizes, num_sizes);
    return 0;
}
