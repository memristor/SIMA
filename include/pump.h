#ifndef PUMP_H
#define PUMP_H

#include "motor_logic.h"
#include "debug.h"

#define PUMP_PIN GPIO_NUM_4

#define START_POSITION 1450
#define BAR_PICKUP_POSITION 1730                                                                                                                                
#define HIGHEST_POSITION 2050

#define BAR_TOLERANCE_SINGLE 190
#define BAR_TOLERANCE_DOUBLE 270                                                                                                                                                                      

extern uint32_t pump_motor_pos;
extern uint32_t pump_motor_gpos;

void setup_pump(void);
void prep_pump(void);
void lift_pump(void);
void starting_pos(void);
void pick_up_bar(bool is_double);
void release_bar(void);
void rotate_and_release_bar(double angle_deg);

#endif