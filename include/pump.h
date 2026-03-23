#ifndef PUMP_H
#define PUMP_H

#include "motor_logic.h"

#define PUMP_PIN GPIO_NUM_4
#define LOWEST_POSITION 1277
#define HIGHEST_POSITION 2250

extern uint32_t pump_motor_pos;
extern uint32_t pump_motor_gpos;

void setup_pump(void);
void rotate_pump(double angle);
void pick_up_bar();
void release_bar();


#endif