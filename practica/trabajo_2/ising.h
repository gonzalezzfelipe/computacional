#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "random_numb.h"

#define M 2147483647


int fill(int* array, int size, int* seed);
float energy(int* array, int size, float j, float b);
float magnetization(int* array, int size);
float flip(
  int* array,
  int size,
  float j,
  float b,
  int* seed,
  float* exps);
int fill_exps(float* exps, float j, float b);


int fill(int* array, int size, int* seed){
  /* Fill Ising array with 1 and -1 using random seed.*/
  int i;
  for (i = 0; i < size * size; i++){
    *(array + i) = (int)(2 * random_numb(seed)) * 2 - 1;
  }
  return 0;
}


float energy(int* array, int size, float j, float b){
  int i, k, right, down, current;
  float energy;

  energy = 0;

  for (i = 0; i < size; i++){
    for (k = 0; k < size; k++){
      current = *(array + i + size * k);
      right = *(array + (size + i + 1) % size + size * k);
      down = *(array + i + size * ((size + k + 1) % size));

      energy += - current * b - j * current * (right + down);
    }
  }
  return energy / size / size;
}


float flip(
  int* array,
  int size,
  float j,
  float b,
  int* seed,
    float* exps){
  /* Flip spin and decide wether to accept new spin configuration.

  Returns the difference of energy between states.*/
  int up, down, left, right, current, i, k;
  float prob, delta;

  i = rand() % size;
  k = rand() % size;

  current = *(array + i + size * k);
  right = *(array + (size + i + 1) % size + size * k);
  left = *(array + (size + i - 1) % size + size * k);
  up = *(array + i + size * ((size + k - 1) % size));
  down = *(array + i + size * ((size + k + 1) % size));

  prob = *(
    exps
    + 5 * (current + 1) / 2  // First five if spin == -1, next five if spin == 1
    + 2 + current * (up + down + left + right) / 2 // If completely unaligned 0
                                                   // If completely aligned, 4.
  );
  if (random_numb(seed) < prob){
    *(array + i + size * k) = - current;
    delta = 2 * b * current - 2 * j * current * (up + down + left + right);
    return delta / size / size;
  }
  else return 0.0;
}


int fill_exps(float* exps, float j, float b){
  /* Fill array with exponentials as to avoid recalculating.

  At every step, you flip a spin and evaluate wether you want to accept it or
  not, using the Metropolis algorithm. This is done by comparing a random number
  (between 0 and 1) to the following coefficient:

    * exp(- U(i + 1) + U(i - 1)), with
      U = - sum(s_i) * b - sum(s_i * s_j)_<i,j> * j

  Given that at each step we only flip 1 spin, the variation in energy can only
  be one of the following values:

    * - 2 * b - 8 * j
    * - 2 * b - 4 * j
    * - 2 * b - 0 * j
    * - 2 * b + 4 * j
    * - 2 * b + 8 * j
    * + 2 * b - 8 * j
    * + 2 * b - 4 * j
    * + 2 * b - 0 * j
    * + 2 * b + 4 * j
    * + 2 * b + 8 * j

  and it depends on the value of the current spin, and 4 neighbours.
  This will be encoded in the following way:

  If the spin is negative, then one should take into consideration the values
  between 0 and 5. Here the position is related in the following way:
    * If (current * (up + down + left + right) == - 4) ==> position = 0
    * If (current * (up + down + left + right) == - 2) ==> position = 1
    * If (current * (up + down + left + right) == 0) ==> position = 2
    * If (current * (up + down + left + right) == 2) ==> position = 3
    * If (current * (up + down + left + right) == 4) ==> position = 4
  Symbolicaly, exp = *(exps + 2 + current * (up + down + left + right) / 2).
  For the positive spin, you have the same but shifted in 5 places.
  */
  int i;

  for (i = 0; i < 5; i++){
    *(exps + i) = exp(+ 2 * b + 4 * (i - 2) * j);
    *(exps + 5 + i) = exp(- 2 * b + 4 * (i - 2) * j);
  }
  return 0;
}


float magnetization(int* array, int size){
  /* Get magnetization of spin array.*/
  int i, mag;
  mag = 0;
  for (i = 0; i < size * size; i++) mag += *(array + i);
  return mag / 1.0 / size / size;
}
