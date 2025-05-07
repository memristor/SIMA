#include <Dynamixel2Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

#include "init.h"
#include "motors.h"
#include "esp-now.h"
#include "cincLogic.h"
#include "strategy.h"
#include "servo.h"
#include "sensor.h"

bool motors_enabled = false;
bool motors_reset = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initializing...");

  initStrat();
  setupESPNOW();
  setupMotors();
  setupSync();
  sensorInit();
  setupServo();

  initTimer();
}

void loop() {

  checkLED();

  if (!prev_cinc && !cinc) {
    Serial.println("CINC pulled");
    motors_enabled = true;
  }
  prev_cinc = cinc;

  if (motors_reset) {
    Serial.println("Resetting motors...");
    resetMotors();
    motors_reset = false;
  }

  if (motors_enabled) {
    Serial.println("Motors enabled: moving...");

    callTimer();

    checkStrat();

    Serial.println("Zavrseno kretanje!");
    motors_enabled = false;
  }
}
