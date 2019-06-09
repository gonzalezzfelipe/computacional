#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "percolacion.h"


int escribir_linea(FILE* fp, int dim, float p, int* frags, int repeticiones){
  // Escribir linea en el csv.
  int i;

  fprintf(fp, "%f", p);

  fprintf(fp, ",\"[");
  for (i = 1; i < 2000; i++) {
    fprintf(fp, "%f,", *(frags + i) / 1.0 / repeticiones);
  }
  fprintf(fp, "0]\"\n");
  return 0;
}


int main(int argc, char** argv) {
  // Argumentos de linea de comando

    int repeticiones; // Cantidad de veces que repito para cada L.
    int* semilla;  // Semilla inicial para el generador de numeros pseudo aleatorios

    semilla = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &repeticiones);
    if (argc == 3) sscanf(argv[2], "%d", &*semilla);
    else *semilla = 1;

  int dim;
  dim = 64;

  int* red;
  int* frags;

  int i, j;
  float p;

  FILE * fp;  // File to write to

  fp = fopen("valores_4.dat","w");
  fprintf(fp, "%s,%s\n", "p", "n_s");

  red = (int*)malloc(dim * dim * sizeof(int));
  frags = (int*)malloc(dim * dim * sizeof(int));

  for (j = 0; j < 200; j++) {

    p = 0.4 + 0.2 * j / 200.0;
    for (i = 0; i < dim * dim; i++) *(frags + i) = 0;  // Limpio frags

    for (i = 0; i < repeticiones; i++) {
        poblar(red, p, dim, semilla);
        clasificar(red, dim);
        fragmentos(red, dim, frags);
      }
    escribir_linea(fp, dim, p, frags, repeticiones);
  }
  free(red);
  fclose (fp);
  return 0;
}
