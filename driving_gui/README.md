#Sima Robot Control

This project enables basic remote control of **Sima** using a browser interface. 
The ESP32 board on Sima serves as a Wi-Fi access point and receives commands from a local web UI.

<img src="https://raw.githubusercontent.com/prokelj/images/main/web_app.png" alt="Web-App" width="600"/>


---

##Requirements

Before you begin, make sure you have [Python 3] installed (https://www.python.org/downloads/).

---

##Setup Instructions

### 1. Flash Code to ESP32

Upload the `sima.txt` code to the ESP32 board located on the Sima.

### 2. Connect to ESP32 Wi-Fi

After starting up the board, connect to the ESP32 network on your computer:

- **SSID:** `RobotControl`
- **Password:** `12345678`

---

## Run the Web Interface

### 1. Open Terminal / Command Prompt

Navigate to the folder containing `index.html`:

```bash
cd path/to/your/folder
```

Run the following command: 

```bash
python -m http.server 8000
```

This starts a local server on port 8000. Now open your browser and navigate to:

[http://localhost:8000]

You should see the robot control interface in your browser.

Use your keyboard to control Sima’s movement:

## Controls

Key    | Action
-------|------------------
W      | Move forward
A      | Turn left
S      | Move backward
D      | Turn right
R      | Reset position

### How Movement Works

- **Hold a key down** to keep the robot moving in the selected direction.
- Releasing the key immediately stops the robot.
- **Position Reset (`R`)**: Resets the robot’s position to ensure it doesn’t exceed motor range.
- The range is large, and alternating directions (forward/backward) reduces the position.

### Adjusting Speed and Acceleration

Key    | Action
-------|------------------
U      | Increase velocity
J      | Decrease velocity
I      | Increase acceleration
K      | Decrease acceleration

You can dynamically change the robot's **speed** and **acceleration** while driving to simulate different conditions (e.g., fast movements or gradual acceleration).

>Note: When stopping (*no key pressed*), Sima stops immediately to avoid being carried by momentum.
