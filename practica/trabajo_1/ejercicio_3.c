#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "percolacion.h"



int main(int argc, char** argv) {
  // Argumentos de linea de comando

    int repeticiones; // Cantidad de veces que repito para cada L.
    int* semilla;  // Semilla inicial para el generador de numeros pseudo aleatorios

    semilla = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &repeticiones);
    if (argc == 3) sscanf(argv[2], "%d", &*semilla);
    else *semilla = 1;

  int* dims;
  dims = (int*)malloc(5 * sizeof(int));
  *(dims + 0) = 4;
  *(dims + 1) = 16;
  *(dims + 2) = 32;
  *(dims + 3) = 64;
  *(dims + 4) = 128;

  float* pcs;
  pcs = (float*)malloc(5 * sizeof(float));
  *(pcs + 0) = 0.561449;
  *(pcs + 1) = 0.587670;
  *(pcs + 2) = 0.592424;
  *(pcs + 3) = 0.592639;
  *(pcs + 4) = 0.592639;

  int* red;

  int i, l, dim, m, _m, validas;
  float pc;

  FILE * fp;  // File to write to

  fp = fopen("valores_3.dat","w");

  fprintf(fp, "%s,%s,%s\n", "L", "masa", "teo");

  for (l = 0; l < 5; l++) {
    dim = *(dims + l);
    pc = *(pcs + l);

    red = (int*)malloc(dim * dim * sizeof(int));
    m = 0;
    validas = 0;

    for (i = 0; i < repeticiones; i++) {
      poblar(red, pc, dim, semilla);
      clasificar(red, dim);
      _m = masa(red, dim);
      if (_m) validas++;
      m += _m;
    }
    fprintf(fp, "%d,%f,%f\n", dim, m / 1.0 / validas, pow(dim, 1.89));
    free(red);
  }

  return 0;
}
