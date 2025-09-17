# MCL & RayCasting – 2D Robot Localization Simulator (C++/SFML)

A lightweight simulator of **Monte Carlo Localization (MCL)** with **LIDAR-style ray casting** in a 2D world.  
Built in modern C++ and SFML, it’s designed for **experimentation**, **teaching**, and **rapid prototyping** of probabilistic robotics.

> **Core idea:** given noisy odometry and range measurements, maintain a cloud of weighted particles to estimate robot pose in a known map.  
> We simulate the range sensor via GPU-friendly ray casting.

---

##  Features

- Particle Filter (**MCL**) with motion + measurement update and resampling  
- Fast **ray casting** against line-segment obstacles (LIDAR emulation)  
- **Interactive visualization** in SFML: robot, particles, rays, obstacles  
- Deterministic and stochastic runs (seed control)  
- Configurable FOV, range, particle count, step sizes  
- Extensible modular code: `Environment`, `Robot`, `Particle`, `main loop`  

**Methodology section below screenshots**
---

**Animations and screenchots**


![MCL_1](https://github.com/user-attachments/assets/0b055c1b-c84b-4d24-931c-8d2071dfb243)

![MC2025-09-1716-08-07-ezgif com-resize](https://github.com/user-attachments/assets/b6243dd1-edcc-48a0-807a-3bc19fd626e4)



## 🧮 Methodology

<img width="1035" height="949" alt="image" src="https://github.com/user-attachments/assets/41f29a06-cb1c-4f38-bd3a-b09a2537d042" />
<img width="1032" height="819" alt="Zrzut ekranu 2025-09-17 155934" src="https://github.com/user-attachments/assets/8205f338-484f-4018-beae-601f31717e55" />

