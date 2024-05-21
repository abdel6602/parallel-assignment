#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// 'local_n' has to be a shared global variable as it is needed by all threads
double local_n;

double f(double x) { return 3 * pow(x, 2.0); }
double fdash(double a, double b) { return (pow(b, 3.0) - pow(a, 3.0)); }
double cumtrapz(double a, double b, double n) {

    double h = (b - a) / n;
    double approx = 0.0;
    approx += f(a) / 2 + f(b) / 2;
    for (int i = 1; i < n; i++) approx += f(a + i * h);
    return h * approx;

}

// Now, we also need to define a struct to be used as the argument for the pthread routine
// This struct will be used to pass data to the routine, and also store output data from it
typedef struct {
    double local_a;        // input variable
    double local_b;        // input variable
    double local_integral; // output variable
} thread_data;

// We need an intermediate function that translates the void* into the data that cumtrapz expects
void* cumtrapz_pthread(void* thread_data_ptr) {

    // First, we need to convert back our thread_data object from the void pointer
    // We do so by casting the pointer from a void pointer to a thread_data pointer
    thread_data* data = (thread_data*) thread_data_ptr;

    // Now we can get local_a and local_b and save the local_integral in the thread_data object
    data->local_integral = cumtrapz(data->local_a, data->local_b, local_n);
    return NULL;

}

// Parallelized implementation for 'p' processes with Pthreads
int main(int argc, char* argv[]) {

    double a = 0, b = 100, n = 512, h = (b - a) / n;

    long thread_count = strtol(argv[1], NULL, 10); // Get num. of threads needed from commandline arg
    local_n = n / thread_count; // Local number of steps

    // Create a handle for each thread
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    // Also, create data structs for each thread
    thread_data* thread_data_structs = malloc(thread_count * sizeof(thread_data));

    // Now, create the threads and pass to them their local_a and local_b values
    for (size_t i = 0; i < thread_count; i++) {

        thread_data_structs[i].local_a = a + i * local_n * h;
        thread_data_structs[i].local_b = thread_data_structs[i].local_a + local_n * h;
        pthread_create(&thread_handles[i], NULL, cumtrapz_pthread, (void*) &thread_data_structs[i]);
    }

    // Finally, accumulate the total integral whenever a thread finishes its execution
    double total_integral = 0.0;
    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
        total_integral += thread_data_structs[i].local_integral;
    }

    printf("Approximate cumulative integral via the trapezoidal method (512 steps): %f\n", total_integral);
    printf("                                                   Analytical integral: %f\n", fdash(a, b));

    free(thread_handles);
    return 0;

}
