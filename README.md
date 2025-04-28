# Sima Code Examples

This repository contains example codes for controlling **Sima**.

The examples demonstrate how to use Dynamixel features and how to adapt them for Sima’s movement and control logic.

<img src="https://raw.githubusercontent.com/prokelj/images/main/sima.jpg" alt="Sima" width="380"/>

---

## Folder Structure

### `examples/`

This folder contains various example codes for controlling Sima.  
These examples showcase the **basic functionalities** of Dynamixel motors and demonstrate how to **configure them specifically for our Sima**.

### `esp_now_enable_signal/`

This folder contains code that sends an **enable signal** to the Sima (the funcionality for multiple SIMAs isn't implemented yet) using the **ESP-NOW protocol**.

Use this to wirelessly trigger Sima’s movement start from a separate ESP32 board.

### `driving_gui/`

This is a **web-based application** for general testing of Sima’s functionality.  
It allows control of Sima using a **laptop and keyboard**, independently of any pre-programmed behavior.

> This GUI does not follow a predefined path—it is meant for **manual testing**.

---

## Requirements

- SIMA
- Arduino IDE or VS Code for flashing examples 

---
