#define MAX 32767
#define MIN 150


typedef enum { blue,
               yellow } Strategy;
Strategy strategy = blue;

void SIMA_SUPERSTAR() {
  
  changeAcceleration(MIN, MIN);
  changeVelocity(MAX, MAX);

  moveMotorsMM(1200, 1200);

  if (strategy == yellow) {
    rotateMotors(100);
  } else {
    rotateMotors(-100);
  }

  moveMotorsMM(-100, -100);
  moveMotorsMM(250, 250);
  changeVelocity(60, 60);
  moveMotorsMM(95, 95);
}
