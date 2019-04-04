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

int poblar(int* red, float p, int dim, int *semilla);
int clasificar(int* red, int dim);
int percola_y_masa(int *red, int dim);

int etiqueta_falsa(int *valor, int *historial, int s1, int s2);
int actualizar(int* valor, int up, int left, int* frag, int* historial);

int printred(int* red, int dim);

int main(int argc, char** argv) {
  int* semilla;
  int repeticiones;

  semilla = (int*)malloc(sizeof(int));

  sscanf(argv[1], "%d", &repeticiones);
  if (argc == 3) sscanf(argv[2], "%d", &*semilla);
  else *semilla = 1;

  int* dims;
  int* red;

  int i, j, l, dim, _masa;
  float p, p_c, p_c_squared, masa;

  dims = (int*)malloc(5 * sizeof(int));
  *dims = 4;
  *(dims + 1) = 16;
  *(dims + 2) = 32;
  *(dims + 3) = 64;
  *(dims + 4) = 128;

  printf("%s,%s,%s,%s\n", "L", "p_c", "var", "masa");
  for (l=0; l<5; l++) {
    dim = *(dims + l);
    p_c = 0.0;
    p_c_squared = 0.0;
    masa = 0.0;

    for (i=0; i<repeticiones; i++) {
      p = 0.5;
      for (j=2; j<12; j++) {

        red = (int*)malloc(dim * dim * sizeof(int));
        poblar(red, p, dim, semilla);
        clasificar(red, dim);

        _masa = percola_y_masa(red, dim);
        if (_masa) p = p - pow(2, -j);
        else p = p + pow(2, -j);

      }
      p_c = p_c + p / repeticiones;
      p_c_squared = p_c_squared + p * p / repeticiones;
      p_c_squared = p_c_squared + p * p / repeticiones;
    }
    p_c = p_c / repeticiones;
    p_c_squared = p_c_squared / repeticiones;
    masa = masa + _masa /repeticiones;
    printf("%d,%f,%f,%f\n", dim, p_c, p_c_squared - pow(p_c, 2), masa);
  }
  return 0;
}


// Auxiliares {{{


// int prueba() {
//   int* semilla;
//
//   semilla = (int*)malloc(sizeof(int));
//   *semilla = 1;
//
//   int* red;
//
//   int i, j, l, dim, _percola;
//   float p, p_c, p_c_squared;
//
//   dim = 5;
//   p = 0.6;
//
//   red = (int*)malloc(dim * dim * sizeof(int));
//   poblar(red, p, dim, semilla);
//   printred(red, dim);
//   clasificar(red, dim);
//   printred(red, dim);
//   _percola = percola_y_masa(red, dim);
//   printf("%d\n", _percola);
//
//   return 0;
// }


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


int pointerrange(int* vec, int start, int end) {
  /*
  Llenar vector puntero con range(0, dim * dim /2)

  Funcion auxiliar para chequear que la red se genera correctamente.
  */
  int i;

  for (i=start; i<end; i=i+1) *(vec + i) = i;
  return 0;
}


// }}}


int percola_y_masa(int *red, int dim) {
  /* Saber si percola o no la red.

  Para determinarlo se toman dos vectores auxiliares de dimension dim,
  uno para la primera fila y otro para la ultima. Se pueblan los en estos
  vectores las posiciones que corresponden a las etiquetas que existan en
  la primera y ultima fila, y se multiplican elemento a elemento. Si la
  suma da distinto de cero, entonces percola.
  */
  int* primera_fila;
  int* ultima_fila;
  int i, suma, etiqueta;

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
  for (i=0; i < dim; i++) suma = suma + *(primera_fila + i) * *(ultima_fila + i);

  i = 0;
  etiqueta = 0;

  while (i < dim) {
    if (*(primera_fila + i) * *(ultima_fila + i)) {
      etiqueta = *(primera_fila + i);
      break;
    }
    i++;
  }

  free(primera_fila);
  free(ultima_fila);

  if (etiqueta) {
    suma = 0;
    for (i=0; i < dim * dim; i++) if (*(red + i) == etiqueta) suma++;
    return suma;
  }
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
