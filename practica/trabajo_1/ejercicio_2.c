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


  int i, l, dim, j, m;
  int* red;
  float p;

  FILE * fp;  // File to write to

  fp = fopen("valores_2_fino.dat","w");

  fprintf(fp, "%s,%s,%s\n", "L", "p", "masa");

  for (l = 0; l < 5; l++) {
    dim = *(dims + l);

    red = (int*)malloc(dim * dim * sizeof(int));

    for (j = 0; j < 100; j++) {
      p = 0.58 + 0.02 * j / 100.0;
      m = 0;

      for (i = 0; i < repeticiones; i++) {
        poblar(red, p, dim, semilla);
        clasificar(red, dim);
        m += masa(red, dim);
      }
      fprintf(fp, "%d,%f,%f\n", dim, p, m / 1.0 / repeticiones);
    }
    free(red);
  }
  free(dims);
  fclose (fp);

  return 0;
}
