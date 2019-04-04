#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int printred(int* red, int dim) {
  /*
  Imprimir la red de percolacion.

  Funcion auxiliar para chequear que la red se genera correctamente.
  */
  int i, j;

  for (i=0; i<dim; i=i+1) {
    for (j=0; j<dim; j=j+1) {
      printf("%2d ", *(red + dim * i + j));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}


int printvec(int* vec, int dim) {
  /*
  Printear vector con puntero de dimension dim.
  */
  int i;

  for (i=0; i < dim; i=i+1) printf("%d\n", *(vec + i));
  return 0;
}


int pointerrange(int* vec, int start, int end) {
  /*
  Llenar vector puntero con range(0, dim * dim /2)

  Funcion auxiliar para chequear que la red se genera correctamente.
  */
  int i;

  for (i=start; i<end; i=i+1) *(vec + i) = i;
  return 0;
}
