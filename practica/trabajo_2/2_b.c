#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "aux.h"
#include "ising.h"

#define M 2147483647
#define L 32

/*
ENUNCIADO:
  Estudie el comportamiento del sistema en una red de 32x32 para B = 0 y
  J \in [0.1, 0.6]. Estimar frecuencia de sampleo adecuada para cada constante
  de acoplamiento empleada. Muestre que las correlaciones se hacen mas
  importantes en el entorno del punto critico.

SOLUCION:
  Para resolver esto se va a realizar un barrido de paso 0.1 entre las J,
  tomando todas las muestras (sin dejar tiempo de termalizacion y con
  frecuencia de sampleo 1). Una vez hecho esto, averiguo cuantos pasos dejar
  para termalizar y determinar la frecuencia de sampleo (ambos ~L^2 = 1024).

  Una vez obtenido el tiempo de termalizacion y la frecuencia de sampleo hacer
  un barrido mas fino (0.01), obteniendo los datos de m^2, E^2, m y E para cada
  paso, para luego analizar la magnetizacion, energia, chi y capacidad
  calorifica.
*/


int main(int argc, char** argv){
  /* Command Line Arguments:

  int samples: Amount of samples of energy to get.
  int termalization: Amount of steps to wait for the array to become
    representative of the temperature. (default = size ** 2)
  int correlation_steps: Amount of steps to ignore as to avoid correlation.
    (Default = size ** 2)
  float step: Step between different J.

  int seed: Random seed. Int between 0 and M.
    (Default = rand() % M)
  */
    int samples, size, termalization, correlation_steps;
    float step;
    int* seed;

    seed = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &samples);
    size = L;
    sscanf(argv[2], "%d", &termalization);
    sscanf(argv[3], "%d", &correlation_steps);
    sscanf(argv[4], "%f", &step);
    if (argc == 6) sscanf(argv[5], "%d", &*seed);
    else *seed = rand() % M;

  FILE * fp;  // File to write to

  int* array;
  float* exps;
  int i, k;
  float delta, e, m;
  float b, j;

  j = 0.1;
  b = 0.0;

  array = (int*)malloc(sizeof(int) * size * size);
  exps = (float*)malloc(sizeof(float) * 10);

  fp = fopen("valores/2_b.dat","w");
  fprintf(fp, "%s,%s,%s,%s,%s\n", "j", "m", "e", "m2", "e2");

  while (j <= 0.6) {
    fill(array, size, seed);
    fill_exps(exps, j, b);

    for (i = 0; i < termalization; i++) flip(array, size, j, b, seed, exps);

    e = energy(array, size, j, b);

    for (i = 0; i < samples; i++){
      for (k = 0; k < correlation_steps + 1; k++){
        delta = flip(array, size, j, b, seed, exps);
        e += delta;
      }
      m = magnetization(array, size);
      fprintf(fp, "%f,%f,%f,%f,%f\n", j, m, e, pow(m , 2), pow(e, 2));
    }
    j += step;
  }
  fclose (fp);
  return 0;
}
