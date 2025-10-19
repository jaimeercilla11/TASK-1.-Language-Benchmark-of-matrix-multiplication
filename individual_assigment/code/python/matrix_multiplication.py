import time

import tracemalloc
import psutil
import os
from typing import List, Tuple

def matrix_multiply(A: List[List[float]], B: List[List[float]]) -> List[List[float]]:
    """
    Basic O(n³) matrix multiplication algorithm.
    """
    n = len(A)
    m = len(B[0])
    p = len(B)
    
    C = [[0.0 for _ in range(m)] for _ in range(n)]
    
    for i in range(n):
        for j in range(m):
            for k in range(p):
                C[i][j] += A[i][k] * B[k][j]
    
    return C

def create_matrix(rows: int, cols: int, value: float = 1.0) -> List[List[float]]:
    """Create a matrix filled with a specific value."""
    return [[value for _ in range(cols)] for _ in range(rows)]

def benchmark_matrix_multiplication(size: int) -> Tuple[float, float, float]:
    """
    Benchmark matrix multiplication for a given size.
    Returns: (execution_time, memory_used_mb, cpu_percent)
    """
    A = create_matrix(size, size, 1.5)
    B = create_matrix(size, size, 2.5)
    
    process = psutil.Process(os.getpid())
    
    tracemalloc.start()
    
    start_time = time.perf_counter()
    cpu_before = process.cpu_percent(interval=0.1)
    
    C = matrix_multiply(A, B)
    
    end_time = time.perf_counter()
    cpu_after = process.cpu_percent(interval=0.1)
    
    current, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    
    execution_time = end_time - start_time
    memory_mb = peak / (1024 * 1024)
    avg_cpu = (cpu_before + cpu_after) / 2
    
    return execution_time, memory_mb, avg_cpu

def run_benchmarks(sizes: List[int]):
    """Run benchmarks for multiple matrix sizes."""
    print(f"{'Size':<10} {'Time (s)':<15} {'Memory (MB)':<15} {'CPU %':<10}")
    print("-" * 70)
    
    results = []
    for size in sizes:
        try:
            exec_time, memory, cpu = benchmark_matrix_multiplication(size)
            results.append((size, exec_time, memory, cpu))
            print(f"{size:<10} {exec_time:<15.4f} {memory:<15.2f} {cpu:<10.2f}")
        except MemoryError:
            print(f"{size:<10} MEMORY ERROR - Size too large")
            break
        except Exception as e:
            print(f"{size:<10} ERROR: {str(e)}")
            break
    
    return results

if __name__ == "__main__":
    test_sizes = [64, 128, 256, 512]
    
    
    results = run_benchmarks(test_sizes)
    
    if results:
        print("\n")
        print("SUMMARY")
        print("=" * 70)
        total_time = sum(r[1] for r in results)
        avg_memory = sum(r[2] for r in results) / len(results)
        print(f"Total execution time: {total_time:.2f} seconds")
        print(f"Average memory usage: {avg_memory:.2f} MB")
        print(f"Largest matrix tested: {results[-1][0]}x{results[-1][0]}")
