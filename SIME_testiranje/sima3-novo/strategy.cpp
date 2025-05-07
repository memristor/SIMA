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

void SIMA3_YELLOW() {
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  delay(200);
  rotateMotors(60);
  checkEndFlag();
  moveMotorsMM(650, 650);
  checkEndFlag();
  rotateMotors(-60);
  checkEndFlag();
  moveMotorsMM(1480, 1480);
  checkEndFlag();
  rotateMotors(-90);
  checkEndFlag();
  moveMotorsMM(100, 100);

  while (true) {
    moveMotors();
  }
}

void SIMA3_BLUE() {
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  delay(200);
  rotateMotors(-60);
  checkEndFlag();
  moveMotorsMM(650, 650);
  checkEndFlag();
  rotateMotors(60);
  checkEndFlag();
  moveMotorsMM(1480, 1480);
  checkEndFlag();
  rotateMotors(90);
  checkEndFlag();
  moveMotorsMM(100, 100);


  while (true) {
    moveMotors();
  }
}
