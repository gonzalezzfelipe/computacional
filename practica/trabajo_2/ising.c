#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "aux.h"
#include "ising.h"

#define M 2147483647


int main(int argc, char** argv){
  /* Command Line Arguments:

  int samples: Amount of samples of energy to get.
  int size: Side of the square spin array.
  float t: Temperature of the sample.
  int termalization: Amount of steps to wait for the array to become
    representative of the temperature. (default = size ** 2)
  int correlation_steps: Amount of steps to ignore as to avoid correlation.
    (Default = size ** 2)

  float j: Energy of correlation between spins. (Default = 1.0)
  float b: Magnetic field. (Default = 0.0)

  int seed: Random seed. Int between 0 and M.
    (Default = rand() % M)
  */
    int samples, size, termalization, correlation_steps;
    float j ,b;
    int* seed;

    seed = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &samples);
    sscanf(argv[2], "%d", &size);
    if (argc >= 4) sscanf(argv[3], "%d", &termalization);
    else termalization = size * size;
    if (argc >= 5) sscanf(argv[4], "%d", &correlation_steps);
    else correlation_steps = size * size;
    if (argc >= 6) sscanf(argv[5], "%f", &j);
    else j = 1.0;
    if (argc >= 7) sscanf(argv[6], "%f", &b);
    else b = 0.0;
    if (argc >= 8) sscanf(argv[7], "%d", &*seed);
    else *seed = rand() % M;
  int* array;
  float* exps;
  int i, k;
  float delta, e, m;

  array = (int*)malloc(sizeof(int) * size * size);
  exps = (float*)malloc(sizeof(float) * 10);

  fill(array, size, seed);
  fill_exps(exps, j, b);

  for (i = 0; i < termalization; i++) flip(array, size, j, b, seed, exps);

  e = energy(array, size, j, b);
  magnetization(array, size);

  // printf("%s,%s,%s,%s,%s\n", "j", "h", "m", "h2", "m2");
  for (i = 0; i < samples; i++){
    for (k = 0; k < correlation_steps; k++){
      delta = flip(array, size, j, b, seed, exps);
      e += delta;
    }
    m = magnetization(array, size);
    printf("%f,%f,%f,%f,%f\n", j, e, m, e * e, m * m);
  }
  return 0;
}
