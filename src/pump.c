#include "pump.h"
#include "driver/gpio.h"
#include "math.h"
#include "freertos/FreeRTOS.h"

#define PUMP_ARC_RADI_mm 55

//static double TICKS_PER_mm = 4095 / (2 * M_PI * PUMP_ARC_RADI_mm);
uint32_t pump_motor_pos = HIGHEST_POSITION;
uint32_t pump_motor_gpos = HIGHEST_POSITION; 


void setup_pump(void)
{
    gpio_reset_pin(PUMP_PIN);
    gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(PUMP_PIN, false);

    prep_pump();
}

void prep_pump(void)
{
    pump_motor_gpos = BAR_PICKUP_POSITION;
    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos); // prep for bar pickup

    do
    {   
        pump_motor_pos = read_present_position(dxl_port_num, MOTOR_3_ID);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    } while (abs((int)(pump_motor_gpos - pump_motor_pos)) > 20);
}


void pick_up_bar(void)
{
    gpio_set_level(PUMP_PIN, true);

    pump_motor_pos = read_present_position(dxl_port_num, MOTOR_3_ID);
    pump_motor_gpos = pump_motor_pos - 250; // lower the pump a bit more to ensure touching the bar 

    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos);

    do
    {
        pump_motor_pos = read_present_position(dxl_port_num, MOTOR_3_ID);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    } while (abs((int)(pump_motor_gpos - pump_motor_pos)) > 20);

    vTaskDelay(750 / portTICK_PERIOD_MS); // make sure bar is picked up

    pump_motor_gpos = HIGHEST_POSITION; // max val to prep for going over edge
    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos);

    do
    {
        pump_motor_pos = read_present_position(dxl_port_num, MOTOR_3_ID);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    } while (abs((int)(pump_motor_gpos - pump_motor_pos)) > 20);
}

void release_bar(void)
{
    gpio_set_level(PUMP_PIN, false); 
}
