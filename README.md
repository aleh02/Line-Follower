# Yeti v4 – Line Follower Robot (Arduino)

Yeti v4 is a high-performance **line follower robot** based on Arduino.  
It uses a 5-sensor infrared array, PID correction and dual-motor control to follow a line smoothly and at high speed.

This repository includes **two versions** of the project:

- **Modular version** → clean, professional C++ structure (multiple .cpp/.h files)
- **Single-file version (`Yeti_v4/`)** → easy to upload and run

---

## 🚗 Features

- 5-sensor analog IR array  
- Automatic sensor calibration  
- PID controller (P + D active)  
- Smooth motor steering  
- LED feedback (line detected / lost)  
- Two buttons:
  - **BTN1** → Calibration
  - **BTN2** → Start race  
- Failsafe direction recovery when the line is lost  

---

## 📁 Repository Structure

📦 root

├── 🧠 main.cpp

├── 🏎️ race.cpp

├── 🏎️ race.h

├── 👁️ sensors.cpp

├── 👁️ sensors.h

├── ⚙️ motors.cpp

├── ⚙️ motors.h

├── 📐 pid.cpp

├── 📐 pid.h

├── 🗂️ utils.h

│

└── 📁 Yeti_v4

└── 📄 Yeti_v4.ino (single-file version)

---

### ✔️ **Use the modular version** if:
- you want a clean professional codebase  
- you use VSCode + PlatformIO  
- you want to extend the robot (new sensors, extra features, etc.)

### ✔️ **Use the single-file version** if:
- you want a quick upload through Arduino IDE  
- you prefer everything in one file  
- you are showing the project to beginners or students

---

## 🧠 How the Robot Works

### **1. Calibration**
Press **BTN1**  
The robot rotates left and right and records per-sensor:

- minimum reflectance  
- maximum reflectance  

This makes readings stable in different lighting conditions.

---

### **2. Line Position Detection**
Every loop:

- Read sensors  
- Normalize values (0–1000)  
- Apply weighted average  
- Center result around `0` (left = negative, right = positive)

If no sensor detects the line → fallback based on last known direction.

---

### **3. PID Correction**
The correction is computed as:

-correction = KPP + KDD + KI*I


Only **P and D** are used (`KI = 0`), which is ideal for fast line followers.

Correction is then applied by reducing one motor’s speed.

---

### **4. Race Mode**
Press **BTN2** to start.  
LED blinks while waiting → release BTN2 → race begins.

Press any button to stop.

---

## 🔧 Hardware Requirements

- Arduino Uno / Nano  
- 2× DC motors + H-bridge driver (L298N or similar)  
- 5× analog IR sensors  
- 2× momentary buttons  
- 1× LED indicator  

### Default Pin Mapping

| Component | Pin |
|----------|-----|
| BTN1 | 7 |
| BTN2 | 6 |
| LED | 13 |
| Motor1 DIR | 2 |
| Motor1 PWM | 3 |
| Motor2 DIR | 4 |
| Motor2 PWM | 5 |
| Sensors | A0 – A4 |

---

## ▶️ Upload Instructions

### **Single-file version**
1. Open `Yeti_v4/Yeti_v4.ino` in Arduino IDE  
2. Select board (e.g. Arduino Uno)  
3. Upload  

### **Modular version (recommended)**

#### **Arduino IDE**
1. Create a sketch
2. Replace all generated files with the `.cpp` + `.h` files  
3. Compile & upload

#### **VSCode + PlatformIO**
1. Create new project  
2. Copy `.cpp` and `.h` files into `src/` and `include/`  
3. Build & upload

---

## ⚙️ Tuning Parameters

Inside the code you can adjust:

| Parameter | Meaning |
|----------|---------|
| `SPEED` | base motor speed |
| `KP` | proportional gain |
| `KD` | derivative gain |
| `KI` | integral gain (usually 0) |
| `P_LINE_MIN` | line detection threshold |

---

## 🏎️ Performance Tips

- Increase **SPEED** for higher speed  
- Raise **KD** if the robot oscillates  
- Raise **KP** for tighter turns  
- Ensure calibration is done on the race track  
- Keep sensors as close to the line as possible  

---

## 📜 License
Open-source, freely usable for education, competition and personal learning.

---

## ✨ Author
Developed and maintained by **Alessandro Han**.
