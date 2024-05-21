#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

const int SIZE = 800;

void parallel_multiply_outer(int m1[SIZE][SIZE], int m2[SIZE][SIZE], int result[SIZE][SIZE]) {
  #pragma omp parallel for
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      result[i][j] = 0; // Initialize to zero before accumulating
      for (int k = 0; k < SIZE; k++) {
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
}

int main() {
  srand(time(NULL)); // Seed random number generator

  int m1[SIZE][SIZE], m2[SIZE][SIZE], result[SIZE][SIZE];

  // Initialize matrices with random values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      m1[i][j] = rand() % 5 + 1; // Random values between 0 and 9
      m2[i][j] = rand() % 5 + 1;
    }
  }

  // Parallel execution (outer loop)
  printf("\nParallel Multiplication (Outer Loop):\n");
  parallel_multiply_outer(m1, m2, result);

  // Printing the entire result matrix for a 500x500 matrix might overwhelm the console. 
//   Uncomment the following if you want to see a small portion.
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%d ", result[i][j]);
    }
    printf("\n");
  }

  return 0;
}
