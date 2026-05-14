#ifndef STRATEGY_H
#define STRATEGY_H

#include "stdbool.h"

#define MAX_VEL_ACC 256
#define MIN_VEL_ACC 1

extern volatile bool end_flag;

void create_move_servo_task();
void create_stop_motors_end_task();
void shut_motors_off();
void SIMA_N_HOMOLOGACIJA();
void SIMA_N_TEST_ROTATION(void);


void SIMA_N_YELLOW_NO_BARS_SAFE();
void SIMA_N_YELLOW_NO_BARS_SAFE_PUSH_FIRST();
void SIMA_N_YELLOW_NO_BARS_AGRESSIVE();
void SIMA_N_YELLOW2();
void SIMA_N_YELLOW4();

void SIMA_N_BLUE_NO_BARS_SAFE();
void SIMA_N_BLUE_NO_BARS_SAFE_PUSH_FIRST();
void SIMA_N_BLUE_NO_BARS_AGRESSIVE();
void SIMA_N_BLUE2();
void SIMA_N_BLUE4();



#endif