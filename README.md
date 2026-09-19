# 5-DOF Robotic Arm Controller & Web Interface

A web-based interface for controlling a 5-DOF robotic arm using the **Web Serial API**. This repository hosts the interactive control interface, motion timeline recording, firmware/code, and mechanical CAD models.

## 🚀 Live Demo
Access the live controller interface via **GitHub Pages**:
> *(GitHub Pages URL will be displayed here after deployment)*

---

## 🦾 Joint / Channel Configuration

| Channel | Joint Name | Range | Default | Function |
|:---:|:---|:---:|:---:|:---|
| **0** | Base Rotation | 0° – 180° | 90° | Base azimuth servo |
| **1** | Shoulder (Arm 1) | 0° – 180° | 90° | Main lift axis |
| **2** | Elbow (Arm 2) | 0° – 180° | 90° | Forward reach / forearm |
| **3** | Wrist Roll | 0° – 180° | 90° | End-effector axial rotation |
| **4** | Wrist Pitch | 0° – 180° | 90° | Gripper tilt / incline |
| **5** | Gripper | 0° – 180° | 90° | Claw open / close |

---

## 📡 Serial Communication Protocol
- **Baud Rate**: 9600 (8-N-1)
- **Format**: channel,value\n
- **Example**: 2,135\n moves channel 2 (Elbow) to 135°.

---

## 📁 Repository Structure
`
.
├── index.html        # Main standalone Web Serial controller UI
├── README.md         # Project documentation and pinout details
├── firmware/         # Arduino / ESP32 source code (to be added)
└── cad/              # 3D models (STL / STEP files) (to be added)
`

---

## 🛠️ Usage
1. Open the hosted GitHub Pages URL in Google Chrome, Microsoft Edge, or Opera (Web Serial API compatible).
2. Click **Connect** and select your microcontroller (Arduino Uno/Nano, ESP32, etc.).
3. Move joint sliders or record keyframes to create coordinated robotic motion timelines.
