#include "sensor.h"

void sensorInit() {
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
}

bool readSensors() {
  return digitalRead(SENSOR1) || digitalRead(SENSOR2);
}
