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

y_max = max(y)
y_min = min(y)
print(y_max,y_min)

plt.plot(x,y,color = 'tab:blue',linewidth = 1.5,label='EffN')
plt.scatter(x[mask_m], y[mask_m], color='orange', s=60, label='Robot movement')
plt.scatter(x[mask_r],y[mask_r], color='red', s=60,label='Partcile resamplment')

plt.xlabel("Epochs")
plt.ylabel("Effective Sample Size")
plt.legend()
plt.grid(True)
plt.show()
