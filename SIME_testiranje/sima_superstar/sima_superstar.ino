#include <Dynamixel2Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

#define stratPin 13

extern volatile bool endFlag;

bool motors_enabled = false;
bool motors_reset = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initializing...");

  pinMode(stratPin, INPUT_PULLUP); //pin za strat

  initializeESPNOW();
  setupMotors();
  setupSync();
  sensorInit();
  setupServo();

  initTimer();
}

void loop() {

 bool strat = digitalRead(stratPin);

 if(!strat){
  Serial.println("Taster nije pritisnut!");
  delay(1000);
 }else{
  Serial.println("Taster je pritisnut!");
  delay(1000);
 }

  if (motors_reset) {
    Serial.println("Resetting motors...");
    resetMotors();
    motors_reset = false;
  }

  if (motors_enabled) {
    Serial.println("Motors enabled: moving...");
    callTimer();
    SIMA_SUPERSTAR();
    if(endFlag){
      while(true){
        moveMotors();
      }
    }
    Serial.println("Zavrseno kretanje!");
    motors_enabled = false;
  }
}
