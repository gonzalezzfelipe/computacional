import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.stats import norm


def get_path(fname):
    return os.path.join(
        '/Users/felipe/git-repos/computacional/practica/trabajo_1/valores',
        fname)


dims = [4, 16, 32, 64, 128]

# Promedios del ejercicio a

df = pd.read_csv(get_path('valores_a.dat'))
probabilidades_criticas = df.groupby('L').agg(['mean', 'std'])

print(probabilidades_criticas)

# # F(P)
#
# for dim in dims:
#     df = pd.read_csv(f'../valores/{dim}.dat')
#     plt.plot(df.p, df['F(p)'], label=f'L = {dim}')
# plt.grid()
# plt.legend()
# plt.show()

# # f(P)
#
# for dim in dims:
#     df = pd.read_csv(get_path(f'{dim}.dat'))
#     plt.plot(df.p, np.gradient(df['F(p)'], df.p), label=f'L = {dim}')
# plt.grid()
# plt.legend()
# plt.show()

# p_c en funcion del ancho.

p_c_mediana = []
p_c_media = []
anchos = []

for dim in dims:
    df = pd.read_csv(get_path(f'{dim}.dat'))
    f = np.gradient(df['F(p)'], df.p)
    mu, std = norm.fit(f)

    p_c_mediana.append(df[df['F(p)'] >= 0.5].p.min())
    p_c_media.append(f.mean())
    anchos.append(f.std())

import pdb; pdb.set_trace()
plt.plot(anchos, p_c_media)
plt.grid()
plt.legend()
plt.show()
