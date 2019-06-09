#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "random_numb.h"
#include "auxiliares.h"


float metropolis(float start, float delta, int samples, float* out, int* seed);


int main(int argc, char** argv) {
  // Argumentos de linea de comando

    int samples;
    int times;
    int* seed;

    seed = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &samples);
    sscanf(argv[2], "%d", &times);
    if (argc == 4) sscanf(argv[3], "%d", &*seed);
    else *seed = 1;

  int i, j;

  float* array;
  array = (float*)malloc(sizeof(float) * samples);

  float delta, start;

  delta = 1.0;
  start = 0.0;

  FILE * fp;

  fp = fopen("valores/importance_sampling.dat", "w");

  for (i = 0; i < times; i++) {
    metropolis(start, delta, samples, array, seed);

    fprintf(fp, "%f", *(array));
    for (j = 0; j < samples; j++) fprintf(fp, ",%f", *(array + j));
    fprintf(fp, "\n");
  }
  fclose (fp);
  return 0;
}


float metropolis(float start, float delta, int samples, float* out, int* seed){
  /* Get samples using metropolis algorith.

  Using a step 'delta', generate 'samples' samples by usign the Metropolis
  algorith in the following way:
    * Generate a trial float for step i + 1, using previous seed plus a random
    value between [-delta; delta]
    * If exp[(trial ^ 2 - x_i ^ 2) / 2] > random_numb, then accept trial as
    x_{i + 1}.
  Returns the acceptation rate.
  */
  float trial, factor;
  int i, accepted;

  accepted = 1;
  *(out) = start;
  i = 1;

  for (i = 0; i < samples; i++) {
    trial = *(out + i - 1) + (2 * random_numb(seed) - 1) * delta;
    factor = exp(-(trial * trial - *(out + i - 1) * *(out + i - 1)) / 2);
    if (factor > random_numb(seed)) {
      *(out + i) = trial;
      accepted++;
    }
    else *(out + i) = *(out + i - 1);
  }
  return accepted / 1.0 / samples;
}
