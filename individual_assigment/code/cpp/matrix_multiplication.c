#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sys/resource.h>

using namespace std;
using namespace chrono;


vector<vector<double>> matrix_multiply(const vector<vector<double>>& A, 
                                       const vector<vector<double>>& B) {
    int n = A.size();
    int m = B[0].size();
    int p = B.size();
    
    vector<vector<double>> C(n, vector<double>(m, 0.0));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return C;
}


vector<vector<double>> create_matrix(int rows, int cols, double value) {
    return vector<vector<double>>(rows, vector<double>(cols, value));
}


double get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    
    #ifdef __APPLE__
        return usage.ru_maxrss / (1024.0 * 1024.0);
    #else
        return usage.ru_maxrss / 1024.0;
    #endif
}


struct BenchmarkResult {
    int size;
    double execution_time;
    double memory_used;
    
    BenchmarkResult(int s, double t, double m) 
        : size(s), execution_time(t), memory_used(m) {}
};


BenchmarkResult benchmark_matrix_multiplication(int size) {
    vector<vector<double>> A = create_matrix(size, size, 1.5);
    vector<vector<double>> B = create_matrix(size, size, 2.5);
    
    double mem_before = get_memory_usage();
    
    auto start_time = high_resolution_clock::now();
    
    vector<vector<double>> C = matrix_multiply(A, B);
    
    auto end_time = high_resolution_clock::now();
    
    double mem_after = get_memory_usage();
    
    duration<double> execution_time = end_time - start_time;
    double memory_mb = mem_after - mem_before;
    
    return BenchmarkResult(size, execution_time.count(), memory_mb);
}


vector<BenchmarkResult> run_benchmarks(const vector<int>& sizes) {
    cout << left << setw(10) << "Size" 
         << setw(15) << "Time (s)" 
         << setw(15) << "Memory (MB)" << endl;
    cout << string(70, '-') << endl;
    
    vector<BenchmarkResult> results;
    
    for (int size : sizes) {
        try {
            BenchmarkResult result = benchmark_matrix_multiplication(size);
            results.push_back(result);
            
            cout << left << setw(10) << size
                 << setw(15) << fixed << setprecision(4) << result.execution_time
                 << setw(15) << fixed << setprecision(2) << result.memory_used
                 << endl;
                 
        } catch (const bad_alloc& e) {
            cout << left << setw(10) << size 
                 << "MEMORY ERROR - Size too large" << endl;
            break;
        } catch (const exception& e) {
            cout << left << setw(10) << size 
                 << "ERROR: " << e.what() << endl;
            break;
        }
    }
    
    return results;
}

int main() {
    vector<int> test_sizes = {64, 128, 256, 512};
        
    vector<BenchmarkResult> results = run_benchmarks(test_sizes);
    
    if (!results.empty()) {
        cout << "\n" << endl;
        cout << "SUMMARY" << endl;
        cout << string(70, '=') << endl;
        
        double total_time = 0.0;
        double total_memory = 0.0;
        
        for (const auto& result : results) {
            total_time += result.execution_time;
            total_memory += result.memory_used;
        }
        
        double avg_memory = total_memory / results.size();
        
        cout << "Total execution time: " << fixed << setprecision(2) 
             << total_time << " seconds" << endl;
        cout << "Average memory usage: " << fixed << setprecision(2) 
             << avg_memory << " MB" << endl;
        cout << "Largest matrix tested: " << results.back().size 
             << "x" << results.back().size << endl;
    }
    
    return 0;
}