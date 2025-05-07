#include "strategy.h"
#include "motors.h"
#include "servo.h"

extern void moveMotors();

void checkEndFlag() {
  if (endFlag) {
    while (true) {
      moveMotors();
    }
  }
}

void SIMA_SUPERSTAR_YELLOW() {
  changeAcceleration(MIN, MIN);
  changeVelocity(250, 250);

  moveMotorsMM(1150, 1150);
  checkEndFlag();
  rotateMotors(100);
  checkEndFlag();
  moveMotorsMM(-120, -120);
  checkEndFlag();
  delay(100);
  moveMotorsMM(270, 270);
  checkEndFlag();
  changeVelocity(60, 60);
  checkEndFlag();
  moveMotorsMM(78, 78);

  while (true) {
    moveMotors();
  }
}

void SIMA_SUPERSTAR_BLUE() {
  changeAcceleration(MIN, MIN);
  changeVelocity(250, 250);

  moveMotorsMM(1150, 1150);
  checkEndFlag();
  rotateMotors(-100);
  checkEndFlag();
  moveMotorsMM(-120, -120);
  checkEndFlag();
  delay(100);
  moveMotorsMM(270, 270);
  checkEndFlag();
  changeVelocity(60, 60);
  checkEndFlag();
  moveMotorsMM(78, 78);

  while (true) {
    moveMotors();
  }
}
