#include <Dynamixel2Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

typedef enum { blue,
               yellow } Strategy;

Strategy strategy = blue;

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

  changeAcceleration(50,50);

}

void loop() {
  if (motors_reset) {
    Serial.println("Resetting motors...");
    resetMotors();
    motors_reset = false;
  }

  if (motors_enabled) {
    Serial.println("Motors enabled: moving...");

    moveMotorsMM(1300, 1300);

    if (strategy == blue) {
      rotateMotors(90);
    } else if (strategy == yellow) {
      rotateMotors(-90);
    }

    moveMotorsMM(-100, -100);
    moveMotorsMM(320, 320);
    changeVelocity(60, 60);
    moveMotorsMM(35, 35);
    motors_enabled = false;

    changeVelocity(MAX_VELOCITY, MAX_VELOCITY);
  }
}
