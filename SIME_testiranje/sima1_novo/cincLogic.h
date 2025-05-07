#ifndef CINC_LOGIC_H
#define CINC_LOGIC_H

#include "strategy.h"

#define yellowPin 5
#define stratPin 13
#define cincPin 15
#define bluePin 22

extern bool prev_cinc;
extern bool cinc;
extern bool strat;

void initStrat();
void checkLED();
void checkStrat();
void flash_blue();
void flash_yellow();

#endif
