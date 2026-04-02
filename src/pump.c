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

    set_goal_position(dxl_port_num, MOTOR_3_ID, HIGHEST_POSITION);
}

void rotate_pump(double angle)
{
    //double arc_mm = 2 * PUMP_ARC_RADI_mm * (angle * (M_PI / 360));
    uint32_t offset = 4095 * (angle / 360);

    pump_motor_pos = read_profile_gposition(dxl_port_num, MOTOR_3_ID);
    pump_motor_gpos = pump_motor_pos - offset;

    if (pump_motor_gpos > HIGHEST_POSITION)
        pump_motor_pos = HIGHEST_POSITION;
    else if (pump_motor_gpos < LOWEST_POSITION)
        pump_motor_gpos = LOWEST_POSITION;
    
    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos);

    do
    {
        pump_motor_pos = read_profile_gposition(dxl_port_num, MOTOR_3_ID);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    } while ((pump_motor_gpos - pump_motor_pos) > 20);
}

void pick_up_bar()
{
    gpio_set_level(PUMP_PIN, true);
}

void release_bar()
{
    gpio_set_level(PUMP_PIN, false);
}