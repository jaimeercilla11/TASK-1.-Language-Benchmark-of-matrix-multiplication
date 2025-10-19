# Matrix Multiplication Performance Analysis


## Project Overview

This project presents a comprehensive performance comparison of matrix multiplication algorithms implemented in three programming languages: Python, Java, and C++. The study analyzes execution time, memory usage, and computational efficiency across different matrix sizes.

## Objectives

- Implement the basic O(n³) matrix multiplication algorithm in Python, Java, and C++
- Benchmark performance with matrices of increasing sizes (64×64 to 512×512)
- Analyze execution time and memory consumption
- Compare language-specific features affecting performance

## Key Results

| Language | Total Time | Avg Memory | Speed vs C++ |
|----------|-----------|-----------|--------------|
| **C++**  | 0.17s     | 0.66 MB   | 1x (baseline) |
| **Java** | 0.77s     | 2.11 MB   | 4.5x slower   |
| **Python** | 262.74s | 2.68 MB   | 1,545x slower |

### Performance by Matrix Size

| Size | Python (s) | Java (s) | C++ (s) |
|------|-----------|----------|---------|
| 64×64 | 0.4938 | 0.0037 | 0.0002 |
| 128×128 | 1.8428 | 0.0033 | 0.0017 |
| 256×256 | 23.6532 | 0.0525 | 0.0171 |
| 512×512 | 236.7505 | 0.7081 | 0.1487 |

## Repository Structure
```
matrix-multiplication-assignment/
├── README.md
└── Individual_Assignment/
    ├── report.pdf                    
    ├── data/                         
    │   ├── python_results.txt
    │   ├── java_results.txt
    │   └── cpp_results.txt
    └── code/                         
        ├── python/
        │   └── benchmark_matrices.py
        ├── java/
        │   └── MatrixBenchmark.java
        └── cpp/
            └── benchmark_matrices.cpp
```



