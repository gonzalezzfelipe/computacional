#include <stdio.h>
#include <math.h>

#define A 16807
#define M 2147483647
#define Q 127773
#define R 2836
#define S 260572
#define T 10000
#define B 10


float random_numb(int *semilla) {
  /*
   Devuelve un numero aleatorio entre 0.0 y 1.0
   a partir de una semilla dada. Se usa el metodo
   de Park y Miller
   */

  int k;
  float x;

  k = (*semilla) / Q;
  *semilla = A * (*semilla - k * Q) - R * k;
  if (*semilla < 0) *semilla += M;

  x = (*semilla)*(1.0 / M);

  return x;
}
