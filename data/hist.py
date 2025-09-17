import pandas as pd
import matplotlib.pyplot as plt
import os

os.chdir(r"C:\Users\alber\Desktop\MC_localization\data")   #make sure you cvhange the warking dir to data dir
print("Working dir: ", os.getcwd())
# Wczytanie CSV
effn_log = pd.read_csv("EffN.csv")

# Histogram wartości EffN
plt.figure(figsize=(8,5))
y = effn_log.to_numpy()[:,1]
plt.hist(y, bins=30, color="tab:blue", edgecolor="black", alpha=0.7)
plt.axvline(0.5*2500, color="red", linestyle="--", linewidth=2, label="0.5 * N")  # próg resamplingu
plt.xlabel("EffN [# effective particles]")
plt.ylabel("Frequency")
plt.title("Histogram wartości EffN")
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.show()
