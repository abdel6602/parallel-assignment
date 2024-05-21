#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x) { return 3 * pow(x, 2.0); }
double fdash(double a, double b) { return (pow(b, 3.0) - pow(a, 3.0)); }
double cumtrapz(double a, double b, double n) {

    double h = (b - a) / n;
    double approx = 0.0;
    approx += f(a) / 2 + f(b) / 2;
    for (int i = 1; i < n; i++) approx += f(a + i * h);
    return h * approx;

}

// Parallelized implementation for 'p' processes with MPI
int main() {

    double local_a, local_b, local_n, local_integral, a = 0, b = 100, n = 1024, h = (b - a) / n;

    int p, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    local_n = n / p; // My local number of steps is the total number of steps divided per number of processes
    local_a = a + my_rank * local_n * h; // My starting step is my local number of steps away from limit 'a'
    local_b = local_a + local_n * h; // My last step is my local number of steps away from my starting step
    local_integral = cumtrapz(local_a, local_b, local_n); // Call the function for my local limits and steps

    if (my_rank != 0) {
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double total_integral = 0.0;
        total_integral += local_integral;
        for (int i = 1; i < p; i++) {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += local_integral;
        }
        printf("Approximate cumulative integral via the trapezoidal method (512 steps): %f\n", total_integral);
        printf("                                                   Analytical integral: %f\n", fdash(a, b));
    }

    MPI_Finalize();
    return 0;

}