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

void SIMA1_YELLOW() {
  changeAcceleration(MAX, MAX);
  changeVelocity(MAX, MAX);

  delay(2000);

  moveMotorsMM(150, 150);
  checkEndFlag();
  changeVelocity(250, 200);
  moveMotorsMM(360, 300);
  checkEndFlag();
  changeVelocity(250, 250);
  moveMotorsMM(340, 340);
   changeVelocity(200, 250);
  checkEndFlag();
  moveMotorsMM(230, 280);

  while (true) {
    moveMotors();
  }
}

void SIMA1_BLUE() {
  changeAcceleration(MAX, MAX);
  changeVelocity(MAX, MAX);

  delay(2000);

  moveMotorsMM(150, 150);
  checkEndFlag();
  changeVelocity(200, 250);
  moveMotorsMM(300, 360);
  checkEndFlag();
  changeVelocity(250, 250);
  moveMotorsMM(340, 340);
   changeVelocity(250, 200);
  checkEndFlag();
  moveMotorsMM(280, 230);

  while (true) {
    moveMotors();
  }
}
