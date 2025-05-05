#define MAX 32767
#define MIN 150

typedef enum { blue,
               yellow } Strategy;

Strategy strategy = blue;

void SIMA1_Strategy() {
  delay(1500);
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);
  moveMotorsMM(150, 150);
  if(strategy == yellow){
    rotateMotors(35);
  }else{
    rotateMotors(-35);
  }
  moveMotorsMM(750, 750);
  if(strategy == yellow){
    rotateMotors(-35);
  }else{
    rotateMotors(35);
  }
  moveMotorsMM(160, 160);
  
}

