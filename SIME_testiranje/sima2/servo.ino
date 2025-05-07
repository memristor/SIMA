#include <ESP32Servo.h>

Servo myservo;

void setupServo(){
  myservo.attach(27);
  myservo.write(30);
}

void moveMotors(){
  myservo.write(30);
  delay(1000);
  myservo.write(150);
  delay(1000);
}