#ifndef PUMP_H
#define PUMP_H

#include "motor_logic.h"


#define PUMP_PIN GPIO_NUM_4
#define BAR_PICKUP_POSITION 2350
#define HIGHEST_POSITION 4095

extern uint32_t pump_motor_pos;
extern uint32_t pump_motor_gpos;

void setup_pump(void);
void prep_pump(void);
void pick_up_bar(void);
void release_bar(void);


#endif