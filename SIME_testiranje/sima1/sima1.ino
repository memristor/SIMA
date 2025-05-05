#include <Dynamixel2Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

bool motors_enabled = false;
bool motors_reset = false;

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

  initTimer();
}

void loop() {

  if (motors_reset) {
    Serial.println("Resetting motors...");
    resetMotors();
    motors_reset = false;
  }

  if (motors_enabled) {
    Serial.println("Motors enabled: moving...");
    callTimer();
    SIMA1_Strategy();
    Serial.println("Zavrseno kretanje!");
    motors_enabled = false;
  }
}
