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
  changeAcceleration(100, 100);
  changeVelocity(1000, 1000);

  moveMotorsMM(500, 500);
  checkEndFlag();
  moveMotorsMM(70, -70);
  checkEndFlag();
  moveMotorsMM(500, 500);
  checkEndFlag();
  moveMotorsMM(-70, 70);
  checkEndFlag();
  moveMotorsMM(1150, 1150);
  checkEndFlag();
  moveMotorsMM(-85, 85);
  checkEndFlag();
  moveMotorsMM(200, 200);

  while (true) {
    moveMotors();
  }
}

void SIMA3_BLUE() {
  changeAcceleration(100, 100);
  changeVelocity(1000, 1000);

  moveMotorsMM(500, 500);
  checkEndFlag();
  moveMotorsMM(-70, 70);
  checkEndFlag();
  moveMotorsMM(500, 500);
  checkEndFlag();
  moveMotorsMM(70, -70);
  checkEndFlag();
  moveMotorsMM(1150, 1150);
  checkEndFlag();
  moveMotorsMM(85, -85);
  checkEndFlag();
  moveMotorsMM(200, 200);

  while (true) {
    moveMotors();
  }
}
