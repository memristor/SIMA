#ifndef SENSOR_H
#define SENSOR_H

#include "driver/gpio.h"

#define SENS1 33
#define SENS2 19

void sensor_init();
void create_check_sensors_task();
bool read_sensors();

#endif