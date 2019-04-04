#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "percolacion.h"

int a(
  int repeticiones,
  int* semilla,
  int precision,
  char filename[20],
  int* dims);
int b(int repeticiones, int* semilla, char filename[20], int* dims);

int main(int argc, char** argv) {
  // Argumentos de linea de comando

    int repeticiones; // Cantidad de veces que repito para cada L.
    int* semilla;  // Semilla inicial para el generador de numeros pseudo aleatorios
    int precision;  // Precision a la que mido (1 / (2 ^ precision))
    char filename_a[20] = "valores_a.csv";  // Nombre del archivo al cual escribir los resultados
    char filename_b[20] = "valores_b.csv";  // Nombre del archivo al cual escribir los resultados

    semilla = (int*)malloc(sizeof(int));

    sscanf(argv[1], "%d", &repeticiones);
    if (argc == 3) sscanf(argv[2], "%d", &*semilla);
    else *semilla = 1;
    if (argc == 4) sscanf(argv[3], "%d", &precision);
    else precision = 12;

  int* dims;
  dims = (int*)malloc(5 * sizeof(int));
  *dims = 4;
  *(dims + 1) = 16;
  *(dims + 2) = 32;
  *(dims + 3) = 64;
  *(dims + 4) = 128;

  // a(repeticiones, semilla, precision, filename_a, dims);
  b(repeticiones, semilla, filename_b, dims);

  free(dims);

  return 0;
}


int a(
    int repeticiones,
    int* semilla,
    int precision,
    char filename[20],
    int* dims) {
  /* Encontrar p_c haciendo una busqueda binaria.

  El objetivo de este ejercicio es encontrar la probabilidad critica arrancando
  de 0.5 y haciendo una busqueda binaria en funcion de si la red percola o no.
  La busqueda debe tener tantos pasos como la precision que uno busca
  (1 / 2 ^ 12) y se debe hacer al menos 27000 veces por tamaño de red para
  obtener resultados validos.
  */
  FILE * fp;  // File to write to

  int* red;

  int i, j, l, dim, percolo;
  float p;

  fp = fopen(filename,"w");

  fprintf(fp, "%s,%s\n", "L", "p_c");
  for (l=0; l<5; l++) {
    dim = *(dims + l);

    for (i=0; i<repeticiones; i++) {
      p = 0.5;
      for (j=2; j<12; j++) {

        red = (int*)malloc(dim * dim * sizeof(int));
        poblar(red, p, dim, semilla);
        clasificar(red, dim);

        percolo = percola(red, dim);
        if (percolo) p = p - pow(2, -j);
        else p = p + pow(2, -j);

      }
      fprintf(fp, "%d,%f\n", dim, p);
    }
  }
  fclose (fp);
  return 0;
}


int b(int repeticiones, int* semilla, char filename[20], int* dims) {
  /* Encontrar p_c haciendo un barrido de las probabilidades, encontrando F(p)

  Para este ejercicio hay que hacer un barrido de paso 0.01 con 27000
  repeticiones por punto para encontrar la distribucion de probabilidad
  acumulada, siendo F(p) en cada punto el promedio de cuantos valores percolan.
  */
  FILE* fp;  // File to write to

  int* red;

  int i, j, l, dim;
  float p, f;

  fp = fopen(filename,"w");

  fprintf(fp, "%s,%s,%s\n", "L", "p", "F(p)");
  for (l=0; l<5; l++) {
    dim = *(dims + l);

    for (j = 1; j < 100; j++){
      p = j / 100.0;
      f = 0.0;
      for (i = 0; i < repeticiones; i++) {
        red = (int*)malloc(dim * dim * sizeof(int));
        poblar(red, p, dim, semilla);
        clasificar(red, dim);
        f += percola(red, dim) / 1.0 / repeticiones;
      }
      fprintf(fp, "%d,%f,%f\n", dim, p, f);
    }
  }
  fclose (fp);
  return 0;
}
