#ifndef STRATEGY_H
#define STRATEGY_H

#define MAX 1023
#define MIN 150

extern volatile bool endFlag;

void checkEndFlag();
void SIMA3_YELLOW();
void SIMA3_BLUE();

#endif
