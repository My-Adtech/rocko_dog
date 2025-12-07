# Rocko Dog Robot 🐕

**Rocko** is an open-source, 8-DOF quadruped robot designed for STEM education and robotics enthusiasts. It features inverse kinematics, active stabilization, and wireless remote control.

![Rocko Robot](https://via.placeholder.com/800x400?text=Rocko+Dog+Robot+Placeholder)

## ✨ Features
*   **8 Degrees of Freedom**: 4 legs with 2 servos each (Hip + Knee).
*   **Inverse Kinematics**: Calculates precise joint angles for smooth movement.
*   **Walking Gaits**: Implements a **Trot** gait for stable locomotion.
*   **Active Stabilization**: Uses an **MPU6050 IMU** with a **PD Controller** to keep the robot level.
*   **Remote Control**: Control via **Bluetooth** or **WiFi** using an ESP32.
*   **Simulation**: Includes a **MATLAB** script to visualize leg trajectories.
*   **3D Printable**: Parametric models generated via **FreeCAD** or **OpenSCAD**.

## 🛠️ Hardware Requirements
*   **Microcontroller**:
    *   **ESP32**: Low-cost, direct WiFi/MQTT.
    *   **Arduino UNO Q**: High-performance, Hybrid (Linux + MCU).
        *   *Architecture*: Linux side handles Cloud/AI, sends commands to MCU via Serial.
*   **Servo Driver**: PCA9685 16-Channel PWM Driver.
*   **Servos**:
    *   **Standard**: 8x **MG996R** (Recommended for strength).
    *   *Micro*: 8x MG90S (Only for very lightweight builds <500g).
*   **IMU**: MPU6050 (Accelerometer + Gyroscope).
*   **Power**:
    *   **Battery**: 2S LiPo (7.4V) or Li-Ion Pack (3500mAh recommended).
    *   **Regulator**: UBEC (5V/6V 3A+) to power the servos.

## 🔌 Wiring
| Component | Pin | ESP32 Pin |
| :--- | :--- | :--- |
| **PCA9685** | SDA | GPIO 21 |
| | SCL | GPIO 22 |
| | VCC | 3.3V / 5V |
| **MPU6050** | SDA | GPIO 21 |
| | SCL | GPIO 22 |
| | VCC | 3.3V |

**Servo Mapping (PCA9685 Channels):**
*   **Front Left**: Hip=0, Knee=1
*   **Front Right**: Hip=2, Knee=3
*   **Back Left**: Hip=4, Knee=5
*   **Back Right**: Hip=6, Knee=7

## � Performance Specifications (MG996R)
*   **Max Payload**: **~1.0 kg - 1.5 kg** (in addition to robot weight).
    *   *Calculation*: 10kg-cm torque / 8cm lever arm = 1.25kg lift per leg. In Trot gait (2 legs), max total lift is ~2.5kg. Subtracting ~1.5kg robot weight leaves ~1kg payload.
*   **Max Speed**: **~25 cm/s** (Trot Gait).
*   **Climbing Ability**:
    *   **Max Incline**: ~20° (limited by foot friction).
    *   **Max Step Height**: ~4 cm (clearance).
*   **Battery Life**: ~45-60 minutes (with 3500mAh 2S LiPo).

## �💻 Software Setup
1.  **Install Arduino IDE**.
2.  **Install Libraries** (Sketch -> Include Library -> Manage Libraries):
    *   `Adafruit PWMServoDriver`
    *   `Adafruit MPU6050`
    *   `Adafruit Unified Sensor`
3.  **Open Project**: Open `examples/RockoRobot/RockoRobot.ino`.
4.  **Upload**: Select your board (e.g., "DOIT ESP32 DEVKIT V1") and upload.

## 🎮 Usage
1.  Power on the robot.
2.  Connect via **Bluetooth Serial** (Device Name: `RockoDog_BT`).
3.  Send commands:
    *   `F`: **Trot Forward**
    *   `S`: **Stabilize** (Active balancing)
    *   `U`: **Stand Up**
    *   `D`: **Sit Down**

## 🖨️ 3D Printing
*   **FreeCAD**: Run the script `examples/rocko_freecad_script.py` inside FreeCAD to generate the Chassis, Femur, and Calf parts tailored for MG996R servos.
*   **OpenSCAD**: Alternatively, use `examples/rocko_model.scad`.

## 📈 Simulation
Run `examples/RockoSimulation.m` in MATLAB to visualize the Inverse Kinematics and Trot Gait trajectory in 3D.

## 📄 License
MIT License. Feel free to modify and share!
