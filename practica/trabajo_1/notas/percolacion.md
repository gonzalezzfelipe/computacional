# Percolacion

## $p_c \simeq 0.5927...$

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

El problema es llenar los nodos con unos con probabilidad $p$. Si $p$ es bajo, la cantidad de unos va a ser chica, ya que la probabilidad de cero es $(1 - p)$.

Es una gran herramienta para el estudio de transiciones de fase.

* Cuando percola? Cuando hay conexion entre los lados.

"Objetivo": Encontrar clusters de primeros vecinos. La aparicion de clusters esta ligado a p. Existe $p_c$ tal que si $p < p_c$ el sistema no percola y si $p > p_c$ *siempre* percola?


La dificultad esta en que se esta simulando con un $L < \infty$, cuando el sistema verdadero tiene $L \rightarrow \infty$.


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
Cuando $L \rightarrow \infty$ la curva se ve asi. Cuando $L \neq \infty$ la curva se suaviza y uno toma un estadistico. No solo se suaviza sino que tambien puede complicarse porque se corre para los costados.

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
  Todo lo que arranque con $\#$, es un comentario, no lo lee el compilador. El precompilador lo lee sin embargo, y se encarga de incluir los paquetes.

* Definiciones:
  * Tambien se pueden definir constantes (**siempre en mayuscula!**)
  ```c
  #define PI 3.141592  // En general es define espacio nombre espacio valor
  ```
  De la misma manera que con los paquetes, son manejadas por el precompilador.

* Declaraciones de funciones
  * Es buena costumbre declarar las funciones a utilizar al principio. La funcion principal tiene que llamarse `main`. Las funciones se declaran como:
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
  * Argumentos de linea de comando: Cualquier cosa que ponga en la linea de comando, van al main:
  ```c
  int main(argc, argv[])
    {
      // ....
      return 0;
    }
  ```
  `argc` viene a ser como la cantidad de argumentos, `argv` los valores.: Ejemplo: `./miprograma 42 3.14156` tiene 3 argumentos (`argc = 3`) y los valores son `[./miprograma, 42, 1.2345]`.

  Todos los argumentos son, en principio, strings. En nuestro ejemplo, si quisieramos convertir el 42 en entero:
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
  El ampersand (`&`) lo que hace es direccionar hacia alguna posicion en memoria. Cuando definio `int purposeOfLife` me guardo esos bytes de memoria, y despues lo guardo ahi. De las misma manera que las funciones se declaran al principio, las variables tambien.

----

Para nuestro caso en particular, queda claro que tenemos 3 grandes funciones
que definir:

* Poblar(red, p, dim)
  * Quiero una funcion que pueble el arreglo con probabilidad p en cada lugar.
* Clasificar
* Percola?

Como defino mi red? Se la puede trabajar como un arreglo aplastado `==>` un vector. En vez de hacer `red[i][j]` hago `red[dim * i + j]`. Para definir a la red (con punteros o sin punteros):
```c
int *red;  // puntero
int red_sin_puntero[dim * dim];
```
Para alocarle la memoria:
```c
int *red;
red = (int*)malloc(dim * dim * sizeof(int))
```
Aca aloca la memoria (`malloc` significa Memory ALLOCation) y despues uno tiene que llenar esos lugares que se alocaron con enteros (0 o 1) Al final de todo, cuando uno no lo necesita, habria que liberara la memoria. Para borrar
```c
free(red);
```
Otra manera de hacerlos con los punteros, es ir directamente a la direccion de memoria que corresponde:
```c
*(red + dim * i + j);
```
Asi trae el entero que se encuentra en la posicion de memoria `red + dim * i + j`. Recordemos que el nombre de la variable me dice el "lugar" de memoria en donde la encuentro para los punteros.

Todo una parte de generacion de numeros enteros que no anote. Pedir apuntes a otra persona.

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

Estrategia: Recorrer el array de la manera que esta escrito. Si esta, poblado y sus vecinos arriba y a la izquierda no, nueva etiqueta. Si alguno esta etiquetado, pero el otro no, asigno esa etiqueta. Si ninguno esta etiquetado, tengo un conflicto de etiquetas.

Para el conflicto, uso Hoshen-Kopelman:

  * Mantengo un historial, de longitud dim * dim.
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ...
  Cuando tengo que corregir ese lugar, lo cambio por - el frag que queda, el menor de los dos del conflicto. Si entran en conflicto las flags 2 y 3, pongo -2 en la posicion 3 del historial y despues al final reemplazo la etiqueta 3 por el 2 (porque esta apuntada con el historial al -2).

#### Estrategia para encontrar $p_c$

Para encontrar $p_c$ hay 2 maneras, una de tipo bisectiva y otra secuencial. Mejor usar la secuencial con aproximadamente 27000 iteraciones por punto.


## Clase 3

Tenemos que en el caso de $L \rightarrow \infty$, vale que $F(p) = \Theta_H (p - p_c)$, por lo que vale que $f(p) = \frac{dF}{dp} = \delta(p - p_c)$, entonces:

$$< p > = \int_0^1 p f(p) dp = p_c$$

$ \Rightarrow $ De aca viene la idea de tomar el valor medio de la distribucion. Sin embargo esto es medio naive, y no funciona muy bien.

Uno termina suponiendo que cuanto mas se parece la curva a la $\Theta_H$ es una cuestion de escala, por lo que hacer crecer $L$ es acercar a $F(p) \rightarrow \Theta_H$.

Planteo que el scaling viene dado por:

$$\Phi(a(L)(p-p_c)), a(L) \simeq L^{\frac{1}{\nu}}$$

Volviendo al valor medio de $p$, ahora tenemos la nueva funcion desconocida $\Phi$ que depende de $\Phi(z = L^{\frac{1}{\nu}} ( p - p_c))$:

$$< p > = \int_0^1 p f(p) dp = \int (z L^{\frac{1}{\nu}} + p_c) \Phi(z) L^{\frac{-1}{\nu}} dz$$
$$< p > = p_c + \int_{z_{min}}^{z_{max}} \Phi(z) dz L^{\frac{-1}{\nu}}$$
$$< p > = p_c + A L^{\frac{-1}{\nu}}$$

Vemos que ahora queda explicito que usar un sistema finito, me da un corrimiento $p_c$ que en principio no puedo determinar. Para sacarnos esa dependencia de encima, lo mejor que podemos hacer es graficar el valor medio de $p$ en funcion de L, en log log.

$$p_c(L) = p_c(\infty) + A L^{\frac{-1}{\nu}}$$

Aplicando el logaritmo a ambos miembros:

$$ln(p_c(\infty) - p_c(L)) = b + m \ln(L)$$

Deberia ser una recta! Sin embargo, para L muy grande, la funcion se plancha, y deja de valer que A no depende de L. Vemos entonces que agrandar la ventana no es necesariamente bueno, ya que me encuentro con estos problemas.

##### Porque pasa esto?

Falla todo porque la distribucion no es simetrica. Cuanto mas comprimido esta, mas simetrico "parece" por lo que tiende a anular la integral que genera $A$. Gracias a la asimetria, podemos saber mas sobre la cuestion al infinito. De alguna manera parecida, se trabaja en teorica 3. Para estudiar el cambio de fase, uno no se para sobre el cambio de fase sino en la proximidad, para poder extrapolar informacion.

##### Como puedo hacer para calcular $p_c$ sin tener en cuenta $\nu$?

Uso la varianza de la distribucion.

$$\sigma ^ 2 = < p^2 > - < p > ^2$$

Si grafico $p_c(L)$ en funcion de $\sigma$, tenemos que en $p_c(\infty)$ deberia ser la ordenada al origen de esta distribucion. Asi se puede extrapolar el valor de la misma y conseguir el $p_c$. En particular vale que:

$$\sigma ^ 2 = (B - A ^ 2) L ^{\frac{-2}{\nu}}$$

Por lo tanto, $\sigma \simeq L ^{\frac{-1}{\nu}} \simeq p_c(L) - p_c(\infty)$, mas alla de una constante, por lo que esperamos que la relacion entre $\sigma$ y $<p>$ sea lineal.

$\rightarrow$ Graficando $<p>(\sigma)$ para los distintos $L$ vale que haciendo un ajuste lineal, la ordenada al origen deberia ser $p_c$.


----

## Clase 4

##### Hipotesis de Scaling:

$$p_c(L) - p_c(\infty) = A L^{\frac{-1}{\nu}}$$

Vamos a insistir mas con esta idea. Voy a buscar despejar $L$.

$$| p_c(L) - p_c(\infty) |^{-\nu} = A^{-\nu} L$$

Me estaria diciendo que existe una relacion geometrica entre el tamaño de la red y la cercania de mi $p_c$ al verdadero $(p_c(\infty))$.

Tiene sentido geometrico. Es entendible que se "anticipe" la transcion para $L < \infty$, porque es mas probable percolar en una red mas corta. Otra manera de pensarlo, es que si percolo para $L = \infty$, antes ya percolo para $L < \infty$ (dicho mal y pronto). Si pienso en la longitud media de los clusters en vez de L, puedo pensar en una relacion del estilo:

$$\xi \propto | p - p_c(\infty)|^{-\nu}$$

Asi defino la magnitud `LONGITUD CARACTERISTICA`. Es aquella longitud que conecta un punto del cluster con otro punto del cluster. Tambien llamada longitud de correlacion.

> `Longitud de correlación`: Distancia promedio entre dos puntos cualquiera que pertenecen al mismo cluster.

Vemos que vale que si $p \rightarrow p_c(\infty) \Rightarrow \xi \rightarrow \infty.$

Tenemos entonces que hay que ser inteligente a la hora de definir el entorno que voy a estudiar, porque se vuelve importante el tamaño de la ventana que uno elige. Ya no se tiene tanta libertad, pasa a existir una ventana adecuada para el estudio del fenomeno.

Hay un problema! En principio no sabemos cual es $p_c{\infty}$. Como lo pongo en terminos de mi ventana? Sobre la ventana si tengo control, y puedo saber cuanto vale $p_c(L)$, haciendo estadistica. Para responder esta pregunta, lo siguiente.

Arranco por aproximarme a $p_c{\infty}$ por izquierda, $p \rightarrow p_c(\infty)^{-}$. Eventualmente me choco con $p_c(L)$ que cumple en general $p_c(L) < p_c(\infty)$. Sin embargo, el "verdadero" sistema no percolo, solo mi ventana. Cuantos serian la cantidad de nodos que pertenecen al cluster percolante (masa)?

$$p < p_c(\infty) \Rightarrow \lim_{L \rightarrow \infty} \frac{M}{L} = 0$$

$$p = p_c(\infty) \Rightarrow \left\{\begin{matrix} \left.\begin{matrix} M < L^2 \\ L < M \end{matrix}\right\} M \simeq L^D; \; 1 < D < 2 \\ \lim_{L \rightarrow \infty} \frac{M}{L^D} \neq 0 \end{matrix}\right.$$


Tenemos que esperamos que la masa del cluster percolante en la red de lado $L$ vaya como $L^D$, siendo $D$ la dimension fractal.

Que pasa si me acerco por derecha? $p \rightarrow p_c(\infty)^{+}$

Me va a pasar que $M \simeq L^d = L^2$. Voy a buscar resumir todo en una formula.

$$M(L, \xi) = L^D m(\frac{l}{\xi}),\;\; \text{con} \; m(x) = \left\{ \begin{matrix} \text{cte} \;\;\; x<1 \\ x^{d - D} \;\; x >> 1 \end{matrix}\right.$$

Es mas comodo sin embargo trabajar con la densidad de masa, $\rho$.

$$\rho = \frac{M}{L} = L^{D-d} m(\frac{L}{\xi})$$

$$\ln \rho = (D - 2)\ln L + \ln \left ( m(\frac{L}{\xi}) \right ) $$

Para distintas probabilidades, se obtienen curvas que para $L$ chicos, son rectas y para grandes son constantes. Siempre debe valer que para cualquier $p$ debe valer la misma pendiente. (Este grafico es el que ama Claudio, hay que saberlo si o si!!!)

![](./images/density_over_l.png)

Esto te hace ver lo siguiente.

* Si estoy en el entorno de $p_c$, para cualquier $p$ la deberia ver rectas paralelas.
* El lugar donde las paralelas se planchan es donde vale que $\frac{L}{\xi}\simeq 1$.

De esta manera me da informacion de que tan lejos estoy de $p_c$ y que tan correcta es mi eleccion de $L$.

Dato: Tenemos que en la funcion de arriba, tenemos 2 partes. La parte de scaling $L^d$ (es lo que dice la teoria, la parte linda) y por otro lado la parte de las limitaciones computacionales $m(\frac{L}{\xi})$. Esto es scaling finito (`scaling` parte teorica, `finito` por la parte de limitaciones computaciones).

----

### Clase 5

Notese que basicamente hay que medir la masa y la probabilidad critica.

###### Dato
> Si te da la memoria, guarda todo lo que puedas. Sirve guardar las configuraciones, por si en algun momento tenes que volver a correr algo.

En la clase pasada vimos que:

$$L \simeq \left |  p_c(L) - p_c(\infty) \right |^{-\nu}$$

$$\xi \simeq \left |  p(L) - p_c(\infty) \right |^{-\nu}$$

$$M = \text{masa del percolante} = L^D m\left( \frac{L}{\xi}\right), \; \text{con} \; m(x) = \left\{ \begin{matrix} \text{cte} \;\;\; x<1 \\ x^{d - D} \;\; x >> 1 \end{matrix}\right.$$

Ademas es interesante medir la intensidad del cluster percolante, $P_{\infty}$.

$$P_{\infty} = \text{intensidad percolante} = \frac{\text{\# de nodos del cluster percolante}}{\text{\# total de nodos}}$$

Para identificar que el grafico de $P_{\infty}(p)$ es efectivamente una transicion de fase hay que identificar 2 cosas:

* Cambio de comportamiento
* Parametro de orden

Viendo $p \rightarrow p_c(\infty)^{+}$, tenemos que:
* Uno platea una hipotesis de _scaling_, diciendo lo siguiente

  $$p = P_\infty + \sum_{s=0}^{<\infty}n_ss,$$

  con $n_s$ el numero de clusters de tamaño $s$. Que pasa si evaluo esto en $p_c$?

  $$p_c = \cancel{P_\infty(p = p_c)}^{\sim 0} + \sum_{s=0}^{<\infty}n_s(p=p_c)s$$

  Supongo que $n_c(p_c) = q_0 s^{-\tau}$. Porque no le planteo una parte de _scaling finito_? En principio, porque estoy analizando simplemente la red infinita. Ademas, porque estoy para justo en $p_c$, y en lo alrededores vale que la parte contribuyente de _scaling finito_ es constante. Tenemos que $P_{\infty}$ es en el entorno de $p_c$ $P_\infty \simeq 0$,

  $$p_c = \sum_{s=0}^\infty q_0 s^{1- \tau}$$

  Para agregarle la flexibilidad de alejarse un poco del $p_c$ y que valgan las cosas agrego una funcion desconocida $f$ tal que

  $$P_\infty = \sum_{s=0}^\infty q_0s^{1-\tau}[1 - f(z)],\; \text{con } z = (p - p_c)s^\sigma,$$

  y pasando a la integral,

  $$= \sum_0^\infty q_0 s^{1-\tau}[1 - f(z)]ds$$

  Lo importante de esto, es que me quedo una ley de escala tal que

  $$P_\infty = (p-p_c)^{\beta},\; \text{con}\; \beta = \frac{\tau - 2}{\sigma}$$

Todo esto surgio de la hipotesis de que $n_s = q_0 s^{-\tau}$. Ya sabemos que para la red unidimensional $\tau=2$ y para Bethe es $\frac{5}{2}$. Para la red cuadrada?

Recordemos las siguientes relaciones. Tenemos que

$$\frac{M}{L^d} \sim \frac{L^D}{L^2} = L^{D-2} \sim |p - p_c|^{\nu(D-2)}$$

Ademas, tambien encontramos que

$$P_\infty \sim (p-p_c)^\beta \Rightarrow \beta = \nu(D-2) = -\nu(D-d)$$

Por lo tanto ademas vale que

$$\frac{\tau - 2}{\sigma} = \nu(d-D)$$

$$\Rightarrow \tau = 2 + \nu \sigma (2-D)$$

Como haria para obtener $\beta$? Tendria que hacer un ajuste sobre el grafico de $P_\infty(p)$. Dentro de un entornito sobre $p_c$ va a valer mi relacion y deberia poder obtener mi valor. Sin embargo, en un sistema finito, el corte no es abrupto y tengo todo suavizado, lo que complica las cosas. Conclusion, es muy mal negocio estimar $\beta$ directamente. Mas vale intentar estimar el $\tau$ y el $\sigma$. Tenemos que

$$\ln n_s = -\tau \ln s + q_0$$

Esto lo puedo sacar con un simple ajuste.

###### Dato
> $D_{\text{Teorico}} = \frac{91}{48}$

Con que nos vamos a chocar en la realidad? Cuando vayamos a hacer el ejercicio 1.d nos vamos a chocar con que $\tau \simeq 1.85$, cuando sabemos que tiene que ser mayor a 2. El problema es que asumimos que $q_0$ no depende de $s$ y en verdad si. Volviendo al problema original, y haciendo algunos reemplazos, vale que

$$q_0 = \frac{p_c}{\sum_{s=0}^\infty s^{1-\tau}} = \frac{p_c}{\Xi(\tau - 1)}$$

con $\Xi$ la funcion de Riemann. Para poder medir esto bien, voy a tener que usar $\chi^2$.

Sabemos que el error cuadratico va a ser:

$$\chi^2 = \sum_{i=1}^N \left | y_i - (-\tau x_i + b(\tau))\right|^2$$

Para encontrar la mejor vario $\tau$, teniendo en cuenta que la ordenada al origen tambien depende de este $\tau$. Es un ajuste lineal, en el sentido que uno ajusta una linea, pero la diferencia con un ajuste "normal" es que tengo un solo parametro $\tau$ que me determina ambos la pendiente y la ordenada. El valor teorico, es

###### Dato
> $\tau_{\text{Teorico}} = 2.05$

Una manera de encontrar $p_c(L)$ es buscar $p$ tal que vale esta ley de potencias, buscando minimizar $\chi^2$ al valor mas chico posible.


###### Dato
> Me tengo que guardar tambien la distribucion de tamaños. Necesito la masa y los $n_s$.
