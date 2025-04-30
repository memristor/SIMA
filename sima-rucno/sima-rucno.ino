#include <Dynamixel2Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

typedef enum { blue,
               yellow } Strategy;

Strategy strategy = blue;

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

  changeAcceleration(50,50);

}

void loop() {
  
    resetMotors();

    delay(3000);

    moveMotorsMM(1300, 1300);

    Serial.println("Zavrseno kretanje!");

  }
}
