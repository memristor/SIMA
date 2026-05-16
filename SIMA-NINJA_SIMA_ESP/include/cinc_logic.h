#ifndef CINC_LOGIC_H
#define CINC_LOGIC_H

#include "strategy.h"

#define YELLOW_PIN  GPIO_NUM_5
#define STRAT_PIN   GPIO_NUM_13
#define CINC_PIN    GPIO_NUM_15
#define BLUE_PIN    GPIO_NUM_22

extern bool prev_cinc;
extern bool cinc;
extern bool strat;

typedef enum { HOMOLOGACIJA, TEST_ROTATION, NO_BARS_SAFE,
     NO_BARS_SAFE_PUSH_FIRST, NO_BARS_AGR, NO_BARS_AGR_TEAMAUTO, NO_BARS_AGR_GRCI, BARS_2, BARS_4
    } strategy_t;

extern strategy_t strategy;

void init_strat();
void check_led();
void check_strat();
void flash_blue();
void flash_yellow();

#endif