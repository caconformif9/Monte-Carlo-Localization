import pandas as pd
import matplotlib.pyplot as plt
import os

os.chdir(r"MC_localization\data")   #make sure you change the working dir to your data dir 
print("Working dir: ", os.getcwd())


df = pd.read_csv("EffN.csv", sep=None, engine="python", header=None)

names = {2:["epoch","EffN"], 3:["epoch","EffN","moved"], 4:["epoch","EffN","moved","resampled"]}
df.columns = names.get(df.shape[1], [f"col{i}" for i in range(df.shape[1])])


for c in df.columns:
    df[c] = pd.to_numeric(df[c], errors="coerce")
df = df.dropna(subset=["epoch","EffN"])

x = df["epoch"].to_numpy()
y = df["EffN"].to_numpy()
moved = df["moved"].to_numpy() if "moved" in df else None
resampled = df["resampled"].to_numpy() if "resampled" in df else None

plt.figure(figsize=(9,4))
plt.hist(y, bins=30, edgecolor="black", alpha=.75)
plt.xlabel("EffN")
plt.ylabel("Frequency")
plt.title("Histogram EffN")
plt.grid(True, alpha=.3)
plt.tight_layout()
plt.show()
