import java.util.ArrayList;
import java.util.List;

public class MatrixBenchmark {

    /**
     * Algoritmo básico de multiplicación de matrices O(n³)
     */
    public static double[][] matrixMultiply(double[][] A, double[][] B) {
        int n = A.length;
        int m = B[0].length;
        int p = B.length;

        double[][] C = new double[n][m];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < p; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        return C;
    }

    /**
     * Crear una matriz llena con un valor específico
     */
    public static double[][] createMatrix(int rows, int cols, double value) {
        double[][] matrix = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = value;
            }
        }
        return matrix;
    }

    /**
     * Clase para almacenar resultados del benchmark
     */
    static class BenchmarkResult {
        int size;
        double executionTime;
        double memoryUsed;

        BenchmarkResult(int size, double executionTime, double memoryUsed) {
            this.size = size;
            this.executionTime = executionTime;
            this.memoryUsed = memoryUsed;
        }
    }

    /**
     * Benchmark de multiplicación de matrices para un tamaño dado
     * Retorna: BenchmarkResult con (tamaño, tiempo_ejecución, memoria_usada)
     */
    public static BenchmarkResult benchmarkMatrixMultiplication(int size) {
        Runtime runtime = Runtime.getRuntime();

        // Forzar recolección de basura antes de la prueba
        runtime.gc();

        // Obtener memoria inicial
        long memBefore = runtime.totalMemory() - runtime.freeMemory();

        // Crear matrices de prueba
        double[][] A = createMatrix(size, size, 1.5);
        double[][] B = createMatrix(size, size, 2.5);

        // Registrar tiempo de inicio
        long startTime = System.nanoTime();

        // Realizar multiplicación de matrices
        double[][] C = matrixMultiply(A, B);

        // Registrar tiempo de fin
        long endTime = System.nanoTime();

        // Obtener memoria final
        long memAfter = runtime.totalMemory() - runtime.freeMemory();

        // Convertir a segundos y MB
        double executionTime = (endTime - startTime) / 1_000_000_000.0;
        double memoryUsed = (memAfter - memBefore) / (1024.0 * 1024.0);

        return new BenchmarkResult(size, executionTime, memoryUsed);
    }

    /**
     * Ejecutar benchmarks para múltiples tamaños de matrices
     */
    public static List<BenchmarkResult> runBenchmarks(int[] sizes) {
        System.out.printf("%-10s %-15s %-15s%n", "Size", "Time (s)", "Memory (MB)");
        System.out.println("----------------------------------------------------------------------");

        List<BenchmarkResult> results = new ArrayList<>();

        for (int size : sizes) {
            try {
                BenchmarkResult result = benchmarkMatrixMultiplication(size);
                results.add(result);

                System.out.printf("%-10d %-15.4f %-15.2f%n",
                        result.size, result.executionTime, result.memoryUsed);

            } catch (OutOfMemoryError e) {
                System.out.printf("%-10d MEMORY ERROR - Size too large%n", size);
                break;
            } catch (Exception e) {
                System.out.printf("%-10d ERROR: %s%n", size, e.getMessage());
                break;
            }
        }

        return results;
    }

    public static void main(String[] args) {
        int[] testSizes = {64, 128, 256, 512};



        // Calentar la JVM
        System.out.println("Calentando la JVM...");
        benchmarkMatrixMultiplication(32);
        System.out.println();

        List<BenchmarkResult> results = runBenchmarks(testSizes);

        // Estadísticas resumen
        if (!results.isEmpty()) {
            System.out.println("\n");
            System.out.println("SUMMARY");
            System.out.println("======================================================================");

            double totalTime = 0.0;
            double totalMemory = 0.0;

            for (BenchmarkResult result : results) {
                totalTime += result.executionTime;
                totalMemory += result.memoryUsed;
            }

            double avgMemory = totalMemory / results.size();
            BenchmarkResult lastResult = results.get(results.size() - 1);

            System.out.printf("Total execution time: %.2f seconds%n", totalTime);
            System.out.printf("Average memory usage: %.2f MB%n", avgMemory);
            System.out.printf("Largest matrix tested: %dx%d%n", lastResult.size, lastResult.size);
        }
    }
}