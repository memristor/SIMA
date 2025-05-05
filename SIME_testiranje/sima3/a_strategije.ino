#define MAX 32767
#define MIN 150

typedef enum { blue,
               yellow } Strategy;
Strategy strategy = blue;

void SIMA3_Yellow() {
  
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  delay(200);

  if(strategy == yellow){
    rotateMotors(60);
  }else{
    rotateMotors(-60);
  }
  moveMotorsMM(650, 650);
  
  if(strategy == yellow){
    rotateMotors(-60);
  }else{
    rotateMotors(60);
  }
  moveMotorsMM(1480, 1480);
  if(strategy == yellow){
    rotateMotors(-90);
  }else{
    rotateMotors(90);
  }
  moveMotorsMM(100, 100);
  
}

