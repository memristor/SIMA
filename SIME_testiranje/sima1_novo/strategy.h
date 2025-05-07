#ifndef STRATEGY_H
#define STRATEGY_H

#define MAX 32767
#define MIN 150

extern volatile bool endFlag;

void checkEndFlag();
void SIMA1_YELLOW();
void SIMA1_BLUE();

#endif
