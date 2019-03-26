import multiprocessing as mp
import subprocess

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

SEMILLAS = list(range(1, 101))
PROBABILITIES = list(map(lambda x: x / 100, range(1, 100)))
LS = [4, 16, 32, 64, 128]


def percolacion(p, l, semilla):
    output = subprocess.run(
        ['./percolacion', str(p), str(l), str(semilla)], capture_output=True)
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
    df = pd.DataFrame({'promedios': promedios, 'varianzas': varianzas})
    df.to_csv(f'valores/{l}.csv')
    print(f'Termino para L = {l}')


# processes = [
#     mp.Process(
#         target=run_percolacion_for_l,
#         args=(l,))
#     for l in LS]
#
# for p in processes:
#     p.start()
#
# for p in processes:
#     p.join()


########################################################################

# Analysis

for l in LS:
    df = pd.read_csv(f'valores/{l}.csv')
    promedios = df['promedios'].values
    varianzas = df['varianzas'].values
    plt.plot(PROBABILITIES, promedios)
    plt.grid()
    plt.show()
