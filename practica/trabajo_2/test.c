#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "aux.h"
#include "ising.h"

#define M 2147483647
#define L 8

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
*/


int main(int argc, char** argv){
  /* Command Line Arguments:

  int samples: Amount of samples of energy to get.
  int amount: Amount of different arrays to analyse.
  float j: Value for J.

  int seed: Random seed. Int between 0 and M.
    (Default = rand() % M)
  */
    int samples, amount;
    float j;
    int* seed;

    seed = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &samples);
    sscanf(argv[2], "%d", &amount);
    sscanf(argv[3], "%f", &j);
    if (argc == 5) sscanf(argv[4], "%d", &*seed);
    else *seed = rand() % M;

  FILE * fp;  // File to write to

  int* array;
  float* exps;
  int i, k, size;
  float delta, e, m, b;
  char filename[50];

  size = L;
  b = .0;

  array = (int*)malloc(sizeof(int) * size * size);
  exps = (float*)malloc(sizeof(float) * 10);

  for (i = 0; i < amount; i++){
    fill(array, size, seed);
    fill_exps(exps, j, b);
    printfloatvec(exps, 10);

    e = energy(array, size, j, b);

    for (k = 0; k < samples; k++){
      delta = flip(array, size, j, b, seed, exps);
      e += delta;
      m = magnetization(array, size);
    }
  printarray(array, size);
  printf("%f\n", m);
  }
  return 0;
}
