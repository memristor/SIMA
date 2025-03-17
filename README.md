*********Prototype for Controlling Dynamixel Motors via ESP-NOW*********

This project demonstrates how to control Dynamixel motors using the ESP-NOW protocol.

-Overview:

	-A master ESP32 (esp-host.ino) sends a control signal packet to predefined MAC addresses.
    
	-Each ESP32 slave device receives the packet and starts executing the motor control logic.
    
	-It is necessary to retrieve and register the MAC address of each ESP32 individually before communication.

-Setup:

	1.Obtain the MAC address of each ESP32 device.
    
	2.Configure the master device (esp-host.ino) with the correct MAC addresses of the slave devices.
    
	3.Flash the code to the master and slave ESP32 boards.
	Upon receiving the signal, the slave devices will initiate motor control operations.
    
