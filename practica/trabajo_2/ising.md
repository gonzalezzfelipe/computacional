# ISING

```
-----------------
| 1 | 1 | 1 |-1 |
-----------------
|-1 | 1 | 1 |-1 |
-----------------
| 1 |-1 | 1 |-1 |
-----------------
| 1 |-1 | 1 |-1 |
-----------------
|-1 | 1 | 1 |-1 |
-----------------
```

$$-\beta H = \frac{J}{kT}\sum_{<i;j>} s_is_j - \frac{\beta}{kT}\sum_i s_i$$

Con que probabilidad acepto cambiar un spin?

$$P_a = \left \{ \begin{matrix} P^* \frac{w(S_{i+1})}{w(S_i)} \;\; \text{si} \; w(S_{i+1}) \leq w(S_i)\\ P^*  \;\; \text{si} \; w(S_{i+1}) > w(S_i)  \end{matrix}\right .$$

Como se que estoy haciendo "buenos" pasos? Chequeo que la correlacion entre los pasos no sea muy alta, sea razonable. Si es muy alta, no cambio mi posicion.

$$C(k) = \frac{<\chi_{i+k}\chi_i>}{<\chi_i^2> - <\chi_i>^2}$$

> Voy a tener que guardarme la tira de probabilidades de aceptacion, para todas las series (o calcular directamente la correlacion en c)

Tengo que conseguir $<C(k)>$. Vamos a tener que hacer al menos 100 corridas. Defino el $\tilde{k}$ interesante tal que $<C(\tilde{k})> = 0.1$. Voy a tomar samples despues cada $\tilde{k}$, asegurando que las muestras van a estar descorrelacionadas.

Las mediciones van a tener esta pinta.

> Termalizacion $(O(n*2))$ $\Rightarrow$ Sample cada $\tilde{k}$.

### Calculo de cantidad de pasos para termalizacion

Vamos a suponer $J = 0$ (sin interaccion.)

$$P_a = \left \{ \begin{matrix} P^* \frac{w(S_{i+1})}{w(S_i)} = P^* \exp(-\beta[H(S_{i+1}) - H(S_i)]) \\ P^* \end{matrix}\right .$$

Tenemos que la probabilidad de flipear $\nu$ espines es (aproximadamente) una binomial.

$$P_k(\nu) = \left ( \begin{matrix}  k \\ \nu \end{matrix} \right) P_a^\nu(1 - P_a)^{k - \nu}$$

Tomamos $k \rightarrow \infty$ y tenemos que $P_a << 1$ ya que esta multiplicado por la probabilidad subyaciente $P^* \propto L^2$ por lo tanto se toma $P_a k \rightarrow \lambda$ (tasa de aceptacion).

Asi, tenemos que

$$P_k(\nu) = \left ( \begin{matrix}  k \\ \nu \end{matrix} \right) P_a^\nu(1 - P_a)^{k - \nu }\simeq \frac{\lambda^\nu}{\nu!}\exp(-\lambda)$$

con $\nu = 0, 1, 2, 3, ... ,\infty$.

Con esta aproximacion, vale que:

$$P(S_i) = \frac{e^{Bs_i}}{e^B + e^{-B}} \Rightarrow <S_i> = \tanh(B)$$

Por lo tanto:

$$<S_i>^2 = \tanh^2B$$

Tenemos que:

$$P(+1 / +1) = \sum_{\nu=0}^{\infty}\frac{\lambda^{2\nu}}{(2\nu)!}e^{\lambda} = e^{-\lambda} \cosh(\lambda)$$

$$P(+1 / -1) = \sum_{\nu=1}^{\infty}\frac{\lambda^{2\nu - 1}}{(2\nu - 1)!}e^{\lambda} = e^{-\lambda} \sinh(\lambda)$$

Por lo que vale que:

$$<S_{i+k}S_i> = S_{-1}S_{-1} P(-1,-1) + ... = e^{-2\lambda}$$

Entonces llegamos a que la correlacion es:

$$C(k) = \frac{e^{-2\lambda} - \tanh^2(B)}{1 - \tanh^2(B)}$$

Recordamos que lo que anotamos como $B$ es en verdad $B^* = \frac{B}{kT} << 1$.

$$\Rightarrow C(k) = e^{-2P_ak} \simeq e^{-2\frac{k}{L^2}}$$

Se saca como conclusion que no solo la termalizacion va como $L^2$, sino que tambien $\tilde{k}.$.

Ademas, haciendo el Taylor,

$$C(k) = \frac{1 - 2\lambda + ... -\tanh^2B}{1 - \tanh^2B} \simeq 1 - \frac{2\lambda} {1 - \tanh^2B}$$

Por lo que $\lambda \simeq 0$.
