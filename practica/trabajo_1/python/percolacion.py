import multiprocessing as mp
import subprocess

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

SEMILLAS = list(range(1, 2001))
PROBABILITIES = list(map(lambda x: x / 100, range(1, 100)))
LS = [4, 16, 32, 64, 128]


def percolacion(p, l, semilla):
    """Correr una red y ver si percola.

    Parameters
    ----------
    p : float
        Probabilidad de poblacion.
    l : int
        Lado de cuadrado de red.
    semilla : int
        Semilla para generador de numero aleatorio.

    Returns
    -------
    int
        1 si percola, 0 si no.

    """
    output = subprocess.run([
        '/Users/felipe/facultad/computacional/practica/trabajo_1/percolacion',
        str(p),
        str(l),
        str(semilla)], capture_output=True)
    return int(output.stdout)


def run_percolacion_for_l(l):
    promedios = np.zeros(len(PROBABILITIES))
    varianzas = np.zeros(len(PROBABILITIES))
    for i, p in enumerate(PROBABILITIES):
        values = np.zeros(len(SEMILLAS))
        for j, semilla in enumerate(SEMILLAS):
            values[j] = percolacion(p, l, semilla)
        promedios[i] = values.mean()
        varianzas[i] = values.var()
        if p == 0.5:
            print(f'50% alcanzado en L = {l}')
    df = pd.DataFrame({
        'probabilidad': PROBABILITIES,
        'promedios': promedios,
        'varianzas': varianzas})
    df.to_csv(f'valores/{l}.csv')
    print(f'Termino para L = {l}')
