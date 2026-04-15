#ifndef SENSOR_H
#define SENSOR_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SENS1 33
#define SENS2 19

extern TaskHandle_t check_sensors_handle;

void sensor_init();
void create_check_sensors_task();
bool read_sensors();

#endif