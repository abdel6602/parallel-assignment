#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x) { return 3 * pow(x, 2.0); }
double fdash(double a, double b) { return (pow(b, 3.0) - pow(a, 3.0)); }
double cumtrapz(double a, double b, double n) {

    double h = (b - a) / n;
    double approx = 0.0;
    approx += f(a) / 2 + f(b) / 2;
    for (int i = 1; i < n; i++) approx += f(a + i * h);
    return h * approx;

}

// We need an intermediate function that calculates the local boundaries for the currently executing thread
void cumtrapz_openmp(double a, double b, double n, double* total_integral) {

    int num_of_threads = omp_get_num_threads();
    int my_thread_num = omp_get_thread_num();
    double h = (b - a) / n; // step size
    double local_n = n / num_of_threads;
    double local_a = a + my_thread_num * local_n * h;
    double local_b = local_a + local_n * h;
    *total_integral += cumtrapz(local_a, local_b, local_n);

}

// Parallelized implementation for 'p' processes with OpenMP
int main(int argc, char* argv[]) {

    double a = 0, b = 100, n = 512;
    long thread_count = strtol(argv[1], NULL, 10);
    double total_integral = 0.0;

    // Each thread will run "cumtrapz_openmp" after this directive
    #pragma omp parallel num_threads(thread_count)
    cumtrapz_openmp(a, b, n, &total_integral);

    printf("Approximate cumulative integral via the trapezoidal method (512 steps): %f\n", total_integral);
    printf("                                                   Analytical integral: %f\n", fdash(a, b));
    return 0;

}
