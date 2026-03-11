#ifndef STRATEGY_H
#define STRATEGY_H

#include "stdbool.h"

#define MAX_VEL_ACC 32767

extern volatile bool end_flag;

void create_move_servo_task();
void create_stop_motors_end_task();
void shut_motors_off();
void SIMA_N_YELLOW();   // Ove dodati za pojedinacnu simu
void SIMA_N_BLUE();

#endif