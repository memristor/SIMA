#include <Dynamixel2Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

typedef enum { blue,
               yellow } Strategy;
Strategy strategy = yellow;

bool motors_enabled = false;
bool motors_reset = false;

#define MAX_VELOCITY 32767

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initializing...");

  initializeESPNOW();
  setupMotors();
  setupSync();
  sensorInit();
  setupServo();

  changeVelocity(MAX_VELOCITY, MAX_VELOCITY);
  changeAcceleration(50, 50);
}

void loop() {
  if (motors_reset) {
    Serial.println("Resetting motors...");
    resetMotors();
    motors_reset = false;
  }

  if (motors_enabled) {
    Serial.println("Motors enabled: moving...");

    moveMotorsMM(350, 350);
    rotateMotors(-35);
     moveMotorsMM(850, 850);
     rotateMotors(-55);
     moveMotorsMM(-400, -400);

    motors_enabled = false;
  }
}
