#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "random_numb.h"
#include "auxiliares.h"

int percola(int *red, int dim);
int poblar(int* red, float p, int dim, int *semilla);
int limpiar_etiquetas(int *red, int *historial, int dim);
int clasificar(int* red, int dim);
int etiqueta_falsa(int *valor, int *historial, int s1, int s2);
int actualizar(int* valor, int up, int left, int* frag, int* historial);


int percola(int *red, int dim) {
  /* Saber si percola o no la red.

  Para determinarlo se toman dos vectores auxiliares de dimension dim,
  uno para la primera fila y otro para la ultima. Se pueblan los en estos
  vectores las posiciones que corresponden a las etiquetas que existan en
  la primera y ultima fila, y se multiplican elemento a elemento. Si la
  suma da distinto de cero, entonces percola.
  */
  int* primera_fila;
  int* ultima_fila;
  int i, suma;

  primera_fila = (int*)malloc(dim * dim / 2 * sizeof(int));
  ultima_fila = (int*)malloc(dim * dim / 2 * sizeof(int));

  // Me aseguro que mis vectores tengan solo ceros
  for (i=0; i < dim * dim / 2; i++) {
    *(primera_fila + i) = 0;
    *(ultima_fila + i) = 0;
  }

  // Pueblo mis vectores de etiquetas
  for (i=0; i < dim; i++) {
    *(primera_fila + *(red + i)) = *(red + i);
    *(ultima_fila + *(red + (dim - 1) * dim + i)) = *(red+ (dim - 1) * dim + i);
  }

  suma = 0;
  for (i=0; i < dim * dim / 2; i++) suma += *(primera_fila + i) * *(ultima_fila + i);

  free(primera_fila);
  free(ultima_fila);

  if (suma) return 1;
  else return 0;
}


int poblar(int* red, float p, int dim, int *semilla) {
  /*
  Poblar la red con 1 o 0 con probabilidad p.

  La funcion poblar recibe como primer argumento a la posicion de memoria
  donde arranca la red, segumendo argumento la probabilidad de que sea 1
  y dim el lado del cuadrado de la red.
  */
  int i;

  for (i=0; i<dim*dim; i=i+1) {
    *(red + i) = 0;
    if (random_numb(semilla) < p) {
      *(red + i) = 1;
    }
  }
  return 0;
}


int limpiar_etiquetas(int *red, int *historial, int dim) {
  int i, s1;

  for (i=0; i < dim * dim; i++) {
    if (*(red + i)) {
      s1 = *(red + i);
      while (*(historial + s1) < 0) s1 = - *(historial + s1);
      *(red + i) = s1;
    }
  }
  return 0;
}


int clasificar(int* red, int dim) {
/*
  Clasificar los distintos clusters en la red.

  Estrategia: Recorrer el array de la manera que esta escrito. Si esta,
  poblado y sus vecinos arriba y a la izquierda no, nueva etiqueta. Si
  alguno esta etiquetado, pero el otro no, asigno esa etiqueta. Si
  ninguno esta etiquetado, tengo un conflicto de etiquetas.

  Para el conflicto, uso Hoshen-Kopelman:

    * Mantengo un historial, de longitud dim * dim.
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ...
    Cuando tengo que corregir ese lugar, lo cambio por - el frag que
    queda, el menor de los dos del conflicto. Si entran en conflicto las
    flags 2 y 3, pongo -2 en la posicion 3 del historial y despues al
    final reemplazo la etiqueta 3 por el 2 (porque esta apuntada con el
    historial al -2).
  */
  int* frag;  // Labeler de fragmentos
  int* historial;  // Puntero al historial
  int up;
  int left;
  int i;
  int j;

  historial = (int*)malloc(dim * dim * sizeof(int));
  frag = (int*)malloc(sizeof(int));
  pointerrange(historial, 0, dim * dim);

  *(frag) = 2;

  up = 0;
  left = 0;
  if (*(red)) {
    actualizar(red, up, left, frag, historial);
  }
  for (j=1; j < dim; j++) {
    left = *(red + j - 1);
    actualizar(red + j, 0, left, frag, historial);
    }
  for (i=1; i < dim; i++) {
    up = *(red + dim * (i - 1));
    left = 0;
    actualizar(red + dim * i, up, left, frag, historial);
    for (j=1; j < dim; j++) {
      up = *(red + dim * (i - 1) + j);
      left = *(red + dim * i + j - 1);
      if (*(red + dim * i + j)) {
        actualizar(red + dim * i + j, up, left, frag, historial);
        }
      }
    }
  limpiar_etiquetas(red, historial, dim);
  free(historial);
  free(frag);
  return 0;
}


int etiqueta_falsa(int *valor, int *historial, int s1, int s2) {
  int minimo, maximo;

  while (*(historial + s1) < 0) {
    s1 = - (*(historial + s1));
  }
  while (*(historial + s2) < 0) {
    s2 = - (*(historial + s2));
  }
  minimo = s1;
  maximo = s2;
  if (minimo > maximo) {
      minimo = s2;
      maximo = s1;
    }
  *valor = minimo;
  *(historial + maximo) = - minimo;
  *(historial + minimo) = minimo;  // En caso de que sean iguales, me salva
  return 0;
}


int actualizar(int* valor, int up, int left, int* frag, int* historial) {
  if (*(valor)) {
    if (up + left) {
      if (up * left) {
        etiqueta_falsa(valor, historial, left, up);
      } else {
        *(valor) = up;
        if (left > up) *(valor) = left;
        }
      } else {
        *(valor) = *frag;
        *frag = *frag + 1;
      }
    }
  return 0;
  }
