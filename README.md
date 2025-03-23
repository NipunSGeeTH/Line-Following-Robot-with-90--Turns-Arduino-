
# 🤖 Line Following Robot with Smooth 90° Turns

Welcome to the **Line Follower Robot** project! This Arduino-based robot is designed to follow a black line using **8 IR sensor array inputs** and can detect and perform **smooth 90-degree turns** with precision.

---

## 📌 Features

- 🚗 **Line following** using **PID control algorithm**.
- 📐 **Automatic 90-degree turning** when detecting full black lines on left or right.
- ⚙️ **Smooth and adjustable speed control** for both left and right motors.
- 🔁 **Turns continue until central sensors detect the line again** for better accuracy.
- 🔍 **Accurate tracking using 8 IR sensors (IR Array)**.

---

## 🔌 Hardware Connections

| Component     | Pin         |
|---------------|-------------|
| IR Sensors    | A0 → A7     |
| Right Motor A | D2          |
| Right Motor B | D3          |
| Right Motor PWM | D8       |
| Left Motor A  | D4          |
| Left Motor B  | D5          |
| Left Motor PWM | D9        |
| Indicator LED | D13         |

---

## 📷 Sensor Setup
This robot uses an **array of 8 IR sensors** (`IR_val[]`) to detect the black line. Each sensor provides digital input (1 = black, 0 = white), and corresponding **weights are applied** to calculate error for the PID control loop.

```cpp
int IR_val[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int IR_weights[8] = {30, 20, 17, 4, -4, -17, -20, -30};
```

These weights help the robot determine **how far off** it is from the center of the line.

---

## 🧠 How It Works

1. 🔍 **Reads IR sensors** to detect the position of the line.
2. 🧮 Calculates error using a **weighted average**.
3. ⚖️ Adjusts motor speeds using **PID control (Proportional, Integral, Derivative)**.
4. ↪️ If a **90-degree turn is detected** (all left or right sensors see black), the robot:
   - Moves a bit forward.
   - Takes a **90-degree turn**.
   - **Keeps turning until middle sensors (IR4 or IR5)** detect the line again.
5. 🚀 Resumes following the line.

---

## 🛠 Tuning Parameters
You can adjust the following variables to fine-tune your robot:

```cpp
float kp = 8;  // Proportional gain
float kd = 10; // Derivative gain
float ki = 0.0; // Integral gain
int MotorBasedspeed = 28; // Base speed of motors
#define maxSpeed 34 // Max allowed motor speed
```

---

## 📂 File Structure

```
📁 LineFollower/
 ├── LineFollower.ino      // Main Arduino code
 └── README.md             // You're reading it now 😄
```

---

## 💡 Tips
- Ensure your IR sensors are **calibrated properly** (adjust the threshold if needed).
- Play with **`delay` times and `MotorBasedspeed`** to get the best movement.
- Use **black electrical tape** on a white surface for better contrast.

---


---

## 📃 License
Feel free to use, modify, and share this project ✨  
> Made with ❤️ for robotics learning and fun!

---

## 🤝 Contributions Welcome
If you improve this project, feel free to open a pull request! 🔧

---

Happy Building! ⚙️🚗💨
