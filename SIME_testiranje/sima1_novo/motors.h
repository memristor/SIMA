#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "init.h"
#include "sensor.h"

void setupMotors();
void changeVelocity(int32_t vel1, int32_t vel2);
void changeAcceleration(int32_t acc1, int32_t acc2);
void moveMotorsMM(double cm1, double cm2);
void rotateMotors(double angle_deg);
void resetMotors();

#endif
