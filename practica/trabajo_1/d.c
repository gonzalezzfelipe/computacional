#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "percolacion.h"


int ultimo_ocupado(int* frags, int len) {
  /* Busca la posicion del ultimo elemento no nulo del vector.*/
  while (*(frags + len - 1) == 0) len--;
  return len;
}


int escribir_linea(FILE* fp, int dim, float p, int* frags, int repeticiones){
  // Escribir linea en el csv.
  int i;

  fprintf(fp, "%d,%f", dim, p);

  fprintf(fp, ",\"[");
  for (i = 1; i < ultimo_ocupado(frags, dim * dim / 2); i++) {
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
  int* frags;

  int i, l, dim;
  float pc;

  FILE * fp;  // File to write to

  fp = fopen("valores_d.dat","w");
  fprintf(fp, "%s,%s,%s\n", "L", "p", "fragmentos");

  for (l = 0; l < 5; l++) {
    dim = *(dims + l);
    pc = *(pcs + l);

    red = (int*)malloc(dim * dim * sizeof(int));
    frags = (int*)malloc(dim * dim * sizeof(int) / 2);

    for (i = 0; i < dim * dim / 2; i++) *(frags + i) = 0;

    for (i = 0; i < repeticiones; i++) {

      red = (int*)malloc(dim * dim * sizeof(int));

      poblar(red, pc, dim, semilla);
      clasificar(red, dim);
      fragmentos(red, dim, frags);

    }
    escribir_linea(fp, dim, pc, frags, repeticiones);
    free(red);
  }
  fclose (fp);

  return 0;
}
