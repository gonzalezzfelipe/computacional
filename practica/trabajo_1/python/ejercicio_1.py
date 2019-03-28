import operator

import numpy as np

from percolacion import percolacion, LS

# Ejercicio 1

# a)
CANTIDAD = 1000
ITERACIONES = 12

pcs = []
vars = []


def get_pc_for_l(l):
    probabilidades_criticas = np.zeros(CANTIDAD)
    for j, semilla in enumerate(range(1, CANTIDAD + 1)):
        p = 0
        i = 1
        operacion = operator.add
        while i <= ITERACIONES:
            p = operacion(p, 2 ** (- i))
            if percolacion(p, LS[-1], semilla):
                operacion = operator.sub
            else:
                operacion = operator.add
            i += 1
        if not j % 100:
            print('{:.0%}'.format(j / CANTIDAD))
        probabilidades_criticas[j] = p
    return probabilidades_criticas.mean(), probabilidades_criticas.var()


print(sum(probabilidades_criticas) / len(probabilidades_criticas))
