# MCL & RayCasting – 2D Robot Localization Simulator (C++/SFML)

A lightweight simulator of **Monte Carlo Localization (MCL)** with **LIDAR-style ray casting** in a 2D world.  
Built in modern C++ and SFML, it’s designed for **experimentation**, **teaching**, and **rapid prototyping** of probabilistic robotics.

> **Core idea:** given noisy odometry and range measurements, maintain a cloud of weighted particles to estimate robot pose in a known map.  
> I simulate the range sensor via GPU-friendly ray casting.

---

##  Features

- Particle Filter (**MCL**) with motion + measurement update and resampling  
- Fast **ray casting** against line-segment obstacles (LIDAR emulation)  
- **Interactive visualization** in SFML: robot, particles, rays, obstacles  
- Deterministic and stochastic runs (seed control)  
- Configurable FOV, range, particle count, step sizes  
- Extensible modular code: `Environment`, `Robot`, `Particle`, `main loop`

- Additional Python scripts to make log charts

**Methodology section below screenshots and description**

---

##  Animations and screenchots


>Figure 1 - Robot being efectivly localized after every move. (2500 particles, 360 deg fov, 90 lidar beams, robot lidar rays viusalisation on and off)

![MCL_1](https://github.com/user-attachments/assets/0b055c1b-c84b-4d24-931c-8d2071dfb243)

---

>Figure 2 - using `rescue()` funcion to apply some randomness to already existing particles (12000 particles, 360 deg fov, 90 lidar beams)

![ezgif com-optimize](https://github.com/user-attachments/assets/72b83c86-4da3-4ed5-9bf5-aad72a386950)

---

>Figure 3 - raycasting used in robot lidar, rays detect nearest obstacle and return the distance, simulating real life lidar (2500 particles, 360 deg fov, 120 lidar beams)

![MC2025-09-1720-27-13-ezgif com-optimize](https://github.com/user-attachments/assets/91f5be7d-8f09-43c9-9e35-438a9e2e398d)

---

>Figure 4 - effective sample size over epochs chart, caunting EffN for each epoch, saving the result to csv and plotting in Matplotlib. We can notice how robot movement and manual particle resamplment influences EffN with high values depicting low accuracy and low values showing accurate results 

<img width="968" height="512" alt="EffN_over_epochs" src="https://github.com/user-attachments/assets/88283e5f-28db-40ba-9c75-36a0b6bf451e" />

---

## Description 

This project is a **2D simulation of the Monte Carlo Localization (MCL) algorithm**, a cornerstone of **probabilistic robotics**, used to estimate a robot’s position in a known map.  
It is written in **C++17** and uses the **SFML 2.6 graphics library** for visualization.

---

###  Simulation Overview
- The environment is randomly populated with rectangular obstacles.  
- A robot is placed in the environment and can be controlled interactively:  
  - **W / S** → move forward / backward  
  - **A / D** → rotate left / right  

The robot is equipped with a configurable **LIDAR sensor**:
- Parameters: field of view (`fov`) and number of beams (`beams_num`)  
- Implemented with **ray casting** using the **AABB slab method**  
- Each ray computes the minimum intersection distance to obstacles  
- Visualized as semi-transparent rays in the simulator window  

To prevent continuous sampling, the LIDAR:
- Only acquires data after the robot moves and then remains idle for at least **2 seconds**  
- Saves measurement data into a CSV file for offline analysis  

---

###  Monte Carlo Localization Workflow
1. **Initialization**  
   A set of particles is spawned randomly across the map.  

2. **Prediction**  
   Particles update their poses according to robot motion (forward/backward steps and rotations).  
   - Planned: extend with Gaussian noise for more realistic motion models.  

3. **Correction (Measurement Update)**  
   Each particle simulates its own LIDAR scan via ray casting.  
   Particle weights are computed based on the difference between **simulated** and **actual** robot measurements.  

4. **Resampling**  
   Particles are resampled probabilistically - higher weights increase survival probability.  
   Current method: **multinomial resampling**  
   Planned: **low-variance/systematic resampling** for better efficiency.  

5. **Recovery / Rescue**  
   - A fraction of particles are always randomized to avoid particle deprivation.  
   - A **rescue operator** can reinitialize part of the colony (useful in the *kidnapped robot problem*).  

---

###  Data Analysis
- After each iteration, the simulator computes the **Effective Sample Size (EffN)**:  
  \[
  N_{\text{eff}} = \frac{1}{\sum_j \tilde{w}_j^2}
  \]
  where \( \tilde{w}_j \) are normalized particle weights.  

- EffN measures how well the particle population approximates the true posterior.  
- Logged to CSV and later processed in **Python with Matplotlib** to generate performance charts.  

---

---

###  Planned Extensions
- Gaussian motion noise for realistic odometry models.  
- Improved likelihood model (Gaussian error instead of absolute difference).  
- Low-variance/systematic resampling for better stability.  
- Config file for easy parameter tuning.  
- SLAM extension (map building + localization).  

---

This simulator serves as both a **learning tool** and a **playground for robotics algorithms**, helping visualize how particle filters converge to the true robot pose in dynamic environments.



##  Methodology

<img width="1035" height="949" alt="image" src="https://github.com/user-attachments/assets/41f29a06-cb1c-4f38-bd3a-b09a2537d042" />
<img width="1032" height="819" alt="Zrzut ekranu 2025-09-17 155934" src="https://github.com/user-attachments/assets/8205f338-484f-4018-beae-601f31717e55" />

