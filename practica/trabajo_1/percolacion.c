#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define A 16807
#define M 2147483647
#define Q 127773
#define R 2836
#define S 260572
#define T 10000
#define B 10

float random_numb(int *p);
int printred(int* red, int dim);
int clasificar(int* red, int dim);
int etiqueta_falsa(int *red, int *historial, int s1, int s2, int i);
int percola(int *red, int dim);


// Auxiliares {{{


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


// }}}


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
  int suma;
  int i;

  primera_fila = (int*)malloc(dim * dim / 2 * sizeof(int));
  ultima_fila = (int*)malloc(dim * dim / 2 * sizeof(int));

  // Me aseguro que mis vectores tengan solo ceros
  for (i=0; i < dim; i++) {
    *(primera_fila + i) = 0;
    *(ultima_fila + i) = 0;
  }

  // Pueblo mis vectores de etiquetas
  for (i=0; i < dim; i++) {
    *(primera_fila + *(red + i)) = *(red + i);
    *(ultima_fila + *(red + (dim - 1) * dim + i)) = *(red+ (dim - 1) * dim + i);
  }

  // Multiplico elemento a elemento, dejando el resultado en suma
  suma = 0;
  for (i=0; i < dim; i++) {
    suma = suma + *(primera_fila + i) * *(ultima_fila + i);
  }
  if (suma) return 1;
  return 0;
}


float random_numb(int *semilla) {
  /*
  Devuelve un numero aleatorio entre 0.0 y 1.0.

  Metodo de Park y Miller.
  */
  int k;
  float x;

  k=(*semilla)/Q;
  *semilla=A*(*semilla-k*Q)-R*k;
  if (*semilla<0) *semilla+=M;

  x=(*semilla)*(1.0/M);

  return x;
  }


int llenarhistorial(int* historial, int dim) {
  /*
  Llenar historial con range(0, dim * dim /2)

  Funcion auxiliar para chequear que la red se genera correctamente.
  */
  int i;

  for (i=0; i<dim * dim /2; i=i+1) *(historial + i) = i;
  return 0;
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
  int frag;  // Labeler de fragmentos
  int* historial;  // Puntero al historial
  int s1;  // Valor de la posicion anterior
  int s2;  // Valor de la posicion arriba
  int i;
  int j;

  historial = (int*)malloc(dim * dim * sizeof(int));
  llenarhistorial(historial, dim);

  frag = 2;
  // Primera fila {{{

  if (*(red)) {
    *(red) = frag;
    frag++;
  }
  for (j=1; j < dim; j++) {
    if (*(red + j)) {
      s1 = *(red + j - 1);
      if (s1) {
        *(red + j) = s1;
        } else {
          *(red + j) = frag;
          frag++;
        }
      }
    }
  // }}}

  for (i=1; i < dim; i++) {
    // Primer elemento {{{
    s1 = 0;
    s2 = *(red + dim * (i - 1)) ;
    if (*(red + dim * i)) {
      if (s2) {
        *(red + dim * i) = s2;
      } else {
        *(red + dim * i) = frag;
        frag++;
      }
    }
    // }}}
    for (j=1; j < dim; j++) {
      s1 = *(red + dim * i + j - 1);
      s2 = *(red + dim * (i - 1) + j);
      if (*(red + dim * i + j)) {
        if (s1 + s2) {
          if (s1 * s2) {
            etiqueta_falsa(red, historial, s1, s2, dim * i + j);
          } else {
            *(red + dim * i + j) = s2;
            if (s1 > s2) *(red + dim * i + j) = s1;
          }
        } else {
          *(red + dim * i + j) = frag;
          frag++;
        }
      }
    }
  }
  limpiar_etiquetas(red, historial, dim);
  return 0;
}


int etiqueta_falsa(int *red, int *historial, int s1, int s2, int i) {
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
  *(red + i) = minimo;
  *(historial + maximo) = - minimo;
  *(historial + minimo) = minimo;  // En caso de que sean iguales, me salva
  return 0;
}


int main(int argc, char** argv) {
  float p;
  int dim;
  int* red;
  int* semilla;
  int _percola;

  semilla = (int*)malloc(sizeof(int));

  sscanf(argv[1], "%f", &p);
  sscanf(argv[2], "%d", &dim);
  sscanf(argv[3], "%d", &*semilla);

  red = (int*)malloc(dim * dim * sizeof(int));

  poblar(red, p, dim, semilla);
  clasificar(red, dim);
  _percola = percola(red, dim);
  printf("%d\n", _percola);
  return 0;
}
