import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('../valores_b.csv')

for l in [4, 16, 32, 64, 128]:
    plt.plot(df.query(f'L == {l}').p, df.query(f'L == {l}')['F(p)'])
    plt.grid()
    plt.show()
