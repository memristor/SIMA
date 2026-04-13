#ifndef SERVO_H
#define SERVO_H

#include "driver/ledc.h"

#define SERVO_PIN GPIO_NUM_27
#define SERVO_DUTY 1129
#define SERVO_FREQ 50 // Hz
#define CYCLE_STEP 14
#define TOTAL_CYCLES 60
#define ITERATION_TIME 10 // ms

void setup_servo();
void move_motor();

#endif