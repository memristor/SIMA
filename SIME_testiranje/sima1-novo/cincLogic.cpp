#include <Arduino.h>
#include "cincLogic.h"

bool prev_cinc = false;
bool cinc = false;
bool strat = false;

void initStrat() {
  pinMode(stratPin, INPUT_PULLUP);
  pinMode(cincPin, INPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void checkLED() {
  cinc = digitalRead(cincPin);

  if (!cinc) {
    digitalWrite(bluePin, LOW);
    digitalWrite(yellowPin, LOW);
    strat = digitalRead(stratPin);
    if (strat) {
      flash_blue();
    } else {
      flash_yellow();
    }
  } else {
    strat = digitalRead(stratPin);
    if (strat) {
      digitalWrite(bluePin, HIGH);
      digitalWrite(yellowPin, LOW);
    } else {
      digitalWrite(bluePin, LOW);
      digitalWrite(yellowPin, HIGH);
    }
  }
}

void checkStrat() {
  strat = digitalRead(stratPin);
  if (strat) {
    SIMA1_BLUE();
  } else {
    SIMA1_YELLOW();
  }
}

void flash_blue() {
  digitalWrite(bluePin, HIGH);
  delay(100);
  digitalWrite(bluePin, LOW);
  delay(100);
}

void flash_yellow() {
  digitalWrite(yellowPin, HIGH);
  delay(100);
  digitalWrite(yellowPin, LOW);
  delay(100);
}
