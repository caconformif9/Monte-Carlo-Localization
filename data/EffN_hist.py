import os
import matplotlib.pyplot as plt
import pandas as pd


os.chdir(r"C:\Users\alber\Desktop\MC_localization\data")   #make sure you cvhange the warking dir to data dir
print("Working dir: ", os.getcwd())

effn_log = pd.read_csv("EffN.csv")
print(effn_log.notna()) #if numbers continue
print(effn_log.head())
effn_log = effn_log.to_numpy()

x = effn_log[:,0]
y = effn_log[:,1]
movement_flag = effn_log[:,2]  
resampled_flag = effn_log[:,3]


mask_m = movement_flag == 1
mask_r = resampled_flag == 1


fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12,5))

ax1.hist(y, bins=30, color="tab:blue", edgecolor="black", alpha=0.7)
ax1.set_xlabel("EffN")
ax1.set_ylabel("Frequency")
ax1.set_title("Histogram EffN ")
ax1.grid(alpha=0.3)


ax2.hist(y[mask_r], bins=20, alpha=0.7, label="Resampled", color="red", edgecolor="black")
ax2.hist(y[~mask_r], bins=20, alpha=0.7, label="Not resampled", color="tab:blue", edgecolor="black")
ax2.set_xlabel("EffN")
ax2.set_title("Resampled / not resampled")
ax2.legend()
ax2.grid(alpha=0.3)

plt.tight_layout()
plt.show()
