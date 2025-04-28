#include <Dynamixel2Arduino.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {blue, yellow} Strategy;
Strategy strategy = blue;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Initializing...");

  setupMotors();
  setupSync();

  resetMotors();

  changeVelocity(32767, 32767);
  changeAcceleration(50, 50);

  delay(5000);
  
  moveMotorsMM(1200, 1200); //mm

  if(strategy == blue){
    rotateMotors(90);
  }else if (strategy == yellow){
    rotateMotors(-90);
  }
  
  moveMotorsMM(-100, -100);

  moveMotorsMM(320, 320);

  changeVelocity(60, 60);

  moveMotorsMM(35, 35);



  //moveMotors(-5.0, 5.0);

  Serial.println("Zavrseno kretanje!");

}

void loop() {
}
