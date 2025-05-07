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

  delay(500);

  changeVelocity(260, 220);
  moveMotorsMM(410, 350);
  changeVelocity(260, 240);
  checkEndFlag();
  moveMotorsMM(220, 200);
  changeVelocity(500, 500);
  moveMotorsMM(-37, 37);
  checkEndFlag();
  moveMotorsMM(750, 750);

  while (true) {
    moveMotors();
  }
}

void SIMA2_BLUE() {
  changeAcceleration(MIN, MIN);
  changeVelocity(220, 260);

  delay(500);
  moveMotorsMM(350, 410);
  changeVelocity(240, 260);
  checkEndFlag();
  moveMotorsMM(200, 220);
  changeVelocity(500, 500);
  moveMotorsMM(37, -37);
  checkEndFlag();
  moveMotorsMM(750, 750);

  while (true) {
    moveMotors();
  }
}
