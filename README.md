# Yeti v4 – Line Follower Robot (Arduino)

Yeti v4 is a **high-performance line follower robot** based on Arduino.  
It uses a 5-sensor infrared array, PID correction and dual-motor control to follow a black or white line at high speed.

This version (`Yeti_v4`) contains a **single-file, fully commented** and easy-to-run codebase, ideal for learning, competitions or portfolio projects.

---

## 🚗 Features

- **5 IR sensors** for line detection  
- **Dynamic calibration** (min/max per sensor)  
- **PID controller** with P + D gains  
- **Smooth proportional steering** (dual DC motors)  
- **Two operating modes**
  - Calibration (BTN1)
  - Race / line following (BTN2)
- **Failsafe detection** when the line is lost  
- **Blinking LED** for status indication

---

## 📁 Project Structure

Yeti_v4/

│

└── Yeti_v4.ino ← Single, fully-commented Arduino file


Everything runs from one file to keep the project clean and simple.

---

## 🔧 Hardware Setup

- **Microcontroller:** Arduino Uno / Nano  
- **Motors:** 2x DC motors with H-bridge driver (e.g., L298N)  
- **Sensors:** 5x IR line sensors (analog)  
- **Buttons:**  
  - BTN1 → Calibration  
  - BTN2 → Start race  
- **LED:** Green status LED (LEDG)

### Pin Mapping (default)

| Component | Pin |
|----------|-----|
| BTN1     | 7 |
| BTN2     | 6 |
| LEDG     | 13 |
| Motor1 DIR | 2 |
| Motor1 PWM | 3 |
| Motor2 DIR | 4 |
| Motor2 PWM | 5 |
| Sensors | A0–A4 |

---

## 🧠 How It Works

### **1. Calibration**
Press **BTN1**.  
The robot rotates left & right while reading sensors and storing:

- `sens_min[x]` → darkest value (background)
- `sens_max[x]` → brightest value (line)

This allows the robot to work in different lighting conditions.

---

### **2. Line Position Detection**
Each frame:

- Sensor values are normalized 0–1000  
- Weighted average computes line offset  
- Result is centered around **0**
  - < 0 → line on the left  
  - > 0 → line on the right  
  - = 0 → centered  

If no sensor detects the line → fallback to last known direction.

---

### **3. PID Correction**
The robot applies:

-correction = KPP + KDD + KI*I

Only **P and D** are used (KI = 0), ideal for fast line following.

Correction adjusts left/right motor speeds smoothly.

---

### **4. Race Mode**
Press **BTN2**, and:

- LED blinks while waiting
- Release button → race starts
- Robot stops when any button is pressed again

---

## ▶️ How to Upload

1. Open `Yeti_v4/Yeti_v4.ino` in Arduino IDE  
2. Select your board (e.g., Arduino Uno)  
3. Upload

That’s it.

---

## ⚙️ Parameters You Can Tune

Edit inside the code:

| Parameter | Meaning |
|----------|---------|
| `SPEED` | Base motor speed (0–1000) |
| `KP` | Proportional gain |
| `KD` | Derivative gain |
| `KI` | Integral gain (usually 0) |
| `P_LINE_MIN` | Sensor threshold for detecting the line |

---

## 🏎️ Performance Tips

- Increase `SPEED` for faster laps  
- Raise `KD` to reduce oscillation  
- Raise `KP` for more aggressive turns  
- Lower `P_LINE_MIN` in dark environments  
- Ensure good calibration before each run  

---

## 📜 License

This project is open-source and free to use for education, competitions and personal learning.

---

## ✨ Author

Developed and maintained by **Alessandro Han**.

