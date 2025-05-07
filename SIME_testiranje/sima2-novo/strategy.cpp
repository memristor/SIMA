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

void SIMA2_YELLOW() {
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  moveMotorsMM(150, 150);
  checkEndFlag();
  rotateMotors(35);
  checkEndFlag();
  moveMotorsMM(720, 720);
  checkEndFlag();
  rotateMotors(-35);
  checkEndFlag();
  moveMotorsMM(550, 550);

  while (true) {
    moveMotors();
  }
}

void SIMA2_BLUE() {
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  moveMotorsMM(150, 150);
  checkEndFlag();
  rotateMotors(-35);
  checkEndFlag();
  moveMotorsMM(720, 720);
  checkEndFlag();
  rotateMotors(35);
  checkEndFlag();
  moveMotorsMM(550, 550);

  while (true) {
    moveMotors();
  }
}
