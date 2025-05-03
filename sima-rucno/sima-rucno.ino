#include <Dynamixel2Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

typedef enum { blue,
               yellow } Strategy;
Strategy strategy = yellow;

#define MAX_VELOCITY 32767

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initializing...");

  setupMotors();
  setupSync();
  sensorInit();
  setupServo();
  resetMotors();

  changeVelocity(MAX_VELOCITY, MAX_VELOCITY);
  changeAcceleration(50, 50);

  moveMotorsMM(350, 350);
  rotateMotors(-35);
  moveMotorsMM(850, 850);
  rotateMotors(-55);
  moveMotorsMM(-400, -400);
}

void loop() {
}
