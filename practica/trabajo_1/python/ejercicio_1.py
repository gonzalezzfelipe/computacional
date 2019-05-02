import os

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def get_path(fname):
    return os.path.join(
        '/Users/felipe/git-repos/computacional/practica/trabajo_1/valores',
        fname)


dims = [4, 16, 32, 64, 128]

# Promedios del ejercicio a

df = pd.read_csv(get_path('valores_a.dat'))
probabilidades_criticas = df.groupby('L').agg('mean')

print(probabilidades_criticas)

# F(P)

# for dim in dims:
#     df = pd.read_csv(f'../valores/{dim}.dat')
#     plt.plot(df.p, df['F(p)'])
# plt.grid()
# plt.show()

# # f(P)
#
# for dim in dims:
#     df = pd.read_csv(get_path(f'{dim}.dat'))
#     plt.plot(df.p, np.gradient(df['F(p)'], df.p))
# plt.grid()
# plt.show()

# f(P)

for dim in dims:
    df = pd.read_csv(get_path(f'{dim}.dat'))
    plt.plot(df.p, np.gradient(df['F(p)'], df.p))
plt.grid()
plt.show()
