#include <stdio.h>
#include <math.h>

// The function whose integration we'd like to approximate
double f(double x) { return 3 * pow(x, 2.0); }

// The analytical integration solution (to compare approximation with exact answer)
double fdash(double a, double b) { return (pow(b, 3.0) - pow(a, 3.0)); }

// The trapezoidal rule for approximating integration between limits 'a' and 'b' in 'n' steps
// The rule is h * [ f(a)/2 + f(x_1) + f(x_2) + ... + f(x_n-1) + f(b)/2 ] as concluded in Ch3
double cumtrapz(double a, double b, double n) {

    // Step size that allows us to take 'n' steps from 'a' to 'b'
    double h = (b - a) / n;

    // Accumulator for integration approximation
    double approx = 0.0;

    // First we add f(a)/2 and f(b)/2 to the accumulator
    approx += f(a) / 2 + f(b) / 2;

    // Then we add the intermediate f(x_i) values, where 'x_i' = a + i * h
    for (int i = 1; i < n; i++) approx += f(a + i * h);

    // Finally, multiply the accumulated value by the step size and return it
    return h * approx;

}

int main() {

    printf("Approximate cumulative integral via the trapezoidal method ( 32 steps): %f\n", cumtrapz(0, 100, 32));
    printf("Approximate cumulative integral via the trapezoidal method (128 steps): %f\n", cumtrapz(0, 100, 128));
    printf("Approximate cumulative integral via the trapezoidal method (512 steps): %f\n", cumtrapz(0, 100, 1024));
    printf("                                                   Analytical integral: %f\n", fdash(0, 100));
    return 0;

}