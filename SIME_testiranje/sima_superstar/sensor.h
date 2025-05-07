#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

#define SENSOR1 19
#define SENSOR2 33

void sensorInit();
bool readSensors();

#endif // SENSOR_H
