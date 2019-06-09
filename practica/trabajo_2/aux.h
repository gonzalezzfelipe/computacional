#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int printarray(int* array, int size) {
  /* Print square int array.*/
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%2d ", *(array + size * i + j));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}


int printfloatvec(float* vec, int size){
  int i;
  for (i = 0; i < size; i++) printf("%f\n", *(vec + i));
  return 0;
}
