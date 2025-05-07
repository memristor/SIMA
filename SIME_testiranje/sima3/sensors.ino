#define SENSOR1 19
#define SENSOR2 33

void sensorInit(){
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
}

bool readSensors(){

  if(digitalRead(SENSOR1) || digitalRead(SENSOR2)){
      return true;
    }else {
      return false;
    }
}
