# Percolacion

Tengo una red del estilo:

```
 --- --- --- --- --- ---
| 1 | 0 | 1 | 1 | 1 | 1 |
 --- --- --- --- --- ---
| 0 | 1 | 0 | 1 | 0 | 1 |
 --- --- --- --- --- ---
| 1 | 0 | 0 | 0 | 0 | 0 |
 --- --- --- --- --- ---
| 1 | 1 | 0 | 0 | 1 | 0 |
 --- --- --- --- --- ---
| 0 | 1 | 1 | 0 | 1 | 1 |
 --- --- --- --- --- ---
```

El problema es llenar los nodos con unos con probabilidad $p$. Si $p$ es bajo,
la cantidad de unos va a ser chica, ya que la probabilidad de cero es $(1 - p)$.

Es una gran herramienta para el estudio de transiciones de fase.

* Cuando percola? Cuando hay conexion entre los lados.

"Objetivo": Encontrar clusters de primeros vecinos. La aparicion de clusters
esta ligado a p. Existe $p_c$ tal que si $p < p_c$ el sistema no percola y si
$p > p_c$ *siempre* percola?


La dificultad esta en que se esta simulando con un $L < \inf$, cuando el
sistema verdadero tiene $L \rightarrow \inf$.


```
Percolacion
  |
  |
1 -..........---------*
  |         |         .
  |         |         .
  |         |         .
  |         |         .
  |         |         .
 -|---------|---------|----> p
                    1
```
Cuando $L \rightarrow \inf$ la curva se ve asi. Cuando $L \neq \inf$ la curva
se suaviza y uno toma un estadistico. No solo se suaviza sino que tambien puede
complicarse porque se corre para los costados.

## Primera pregunta

Queremos hacer un programa para encontrar el $p_c$. Antes, un repaso de C.

----
### Repaso de  C

Los programas de C tienen basicamente 3 partes:

* Paquetes:
  ```c
  #include<stdio.h>  // Standard in and out
  #include<stdlib.h>  // Standard library
  #include<math.h>  // No indispensable, pero si para nosotros en general
  ```
  Todo lo que arranque con $\#$, es un comentario, no lo lee el compilador. El
  precompilador lo lee sin embargo, y se encarga de incluir los paquetes.

* Definiciones:
  * Tambien se pueden definir constantes (**siempre en mayuscula!**)
  ```c
  #define PI 3.141592  // En general es define espacio nombre espacio valor
  ```
  De la misma manera que con los paquetes, son manejadas por el precompilador.

* Declaraciones de funciones
  * Es buena costumbre declarar las funciones a
utilizar al principio. La funcion principal tiene que llamarse `main`.
Las funciones se declaran como:
    1. Que va a devolver. Que devuelva a void salta a warning en general.
    2. Como se llama.
    3. El codigo entre corchetes.

  ```c
  int main()
    {
      // ....
      return 0
    }
  ```
  * Argumentos de linea de comando: Cualquier cosa que ponga en la linea de
  comando, van al main:
  ```c
  int main(argc, argv[])
    {
      // ....
      return 0;
    }
  ```
  `argc` viene a ser como la cantidad de argumentos, `argv` los valores.:
  Ejemplo: `./miprograma 42 3.14156` tiene 3 argumentos (`argc = 3`) y los
  valores son `[./miprograma, 42, 1.2345]`.

  Todos los argumentos son, en principio, strings. En nuestro ejemplo, si
  quisieramos convertir el 42 en entero:
  ```c
  int main(argc, argv[])
    {
      int purposeOfLife;
      float alpha;
      sscanf(argv[1], "%d", &purposeOfLife);
      sscanf(argv[2], "%f", &alpha);
      // ....
      return 0;
    }
  ```
  El ampersand (`&`) lo que hace es direccionar hacia alguna posicion en
  memoria. Cuando definio `int purposeOfLife` me guardo esos bytes de memoria,
  y despues lo guardo ahi. De las misma manera que las funciones se declaran al principio, las variables tambien.

----

Para nuestro caso en particular, queda claro que tenemos 3 grandes funciones
que definir:

* Poblar(red, p, dim)
  * Quiero una funcion que pueble el arreglo con probabilidad p en cada lugar.
* Clasificar
* Percola?

Como defino mi red? Se la puede trabajar como un arreglo aplastado `==>` un
vector. En vez de hacer `red[i][j]` hago `red[dim * i + j]`. Para definir a
la red (con punteros o sin punteros):
```c
int *red;  // puntero
int red_sin_puntero[dim * dim];
```
Para alocarle la memoria:
```c
int *red;
red = (int*)malloc(dim * dim * sizeof(int))
```
Aca aloca la memoria (`malloc` significa Memory ALLOCation) y despues uno tiene que llenar esos lugares que se alocaron con enteros (0 o 1) Al final de
todo, cuando uno no lo necesita, habria que liberara la memoria. Para borrar
```c
free(red);
```
Otra manera de hacerlos con los punteros, es ir directamente a la direccion
de memoria que corresponde:
```c
*(red + dim * i + j);
```
Asi trae el entero que se encuentra en la posicion de memoria `red + dim * i + j`.
Recordemos que el nombre de la variable me dice el "lugar" de memoria en donde
la encuentro para los punteros.

Todo una parte de generacion de numeros enteros que no anote. Pedir apuntes a
otra persona.

Para compilar:

```shell
gcc /
  -Wall /  # Te tira todos los warnings.
  -O3 /  # O de optimizacion. 3 es el mas grande que funciona en todos lados
  -o program /  # output, el nombre del programa
  cod.c /  # Archivo con el codigo
  -lm  # para que use las libs de matematica
```
En este orden!

### Clasificacion

#### Algoritmo de Hoshen - Kopelman

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

#### Estrategia para encontrar $p_c$

Para encontrar $p_c$ hay 2 maneras, una de tipo bisectiva y otra secuencial.
Mejor usar la secuencial con aproximadamente 27000 iteraciones por punto.
