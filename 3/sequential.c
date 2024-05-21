#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SIZE = 800;

void sequential_multiply(int m1[SIZE][SIZE], int m2[SIZE][SIZE], int result[SIZE][SIZE]) {
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

  // Sequential execution
  printf("\nSequential Multiplication:\n");
  sequential_multiply(m1, m2, result);

  // Printing the entire result matrix for a 500x500 matrix might overwhelm the console. 
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%d ", result[i][j]);
    }
    printf("\n");
  }

  return 0;
}
