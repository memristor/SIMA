#include "pump.h"
#include "driver/gpio.h"
#include "math.h"
#include "freertos/FreeRTOS.h"

uint32_t pump_motor_pos;
uint32_t pump_motor_gpos; 

const TickType_t timeout = pdMS_TO_TICKS(2000); // timeout if the motor doesn't reach end position -> 2s

void move_pump_motor(uint16_t goal_pos, uint16_t velocity, uint8_t motor_id)
{ 
    if(read_profile_velocity(dxl_port_num, motor_id) != velocity)
    {
        set_profile_velocity(dxl_port_num, motor_id, velocity); // 0 - 1027 range where 0 is max speed
    } else 
    {
        print("Velocity is already at that value.");
    }
    
    // starting position
    pump_motor_gpos = goal_pos;
    set_goal_position(dxl_port_num, motor_id, pump_motor_gpos);

    TickType_t start_time = xTaskGetTickCount();

    while(1)
    {
        pump_motor_pos = read_present_position(dxl_port_num, motor_id);

        if((abs((int)(pump_motor_gpos - pump_motor_pos)) < 20) || ((xTaskGetTickCount() - start_time) >= timeout))
        {
            break;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}   

void setup_pump(void)
{
    gpio_reset_pin(PUMP_PIN);
    gpio_set_direction(PUMP_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(PUMP_PIN, false);

    move_pump_motor(START_POSITION, 80, MOTOR_3_ID);
}

void prep_pump(void)
{
    pump_motor_gpos = BAR_PICKUP_POSITION;
    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos); // prep for bar pickup, no need to wait for it can be done in parallel
}

void starting_pos(void){
    move_pump_motor(START_POSITION, 80, MOTOR_3_ID); // end of strategy, move pump back to starting position
}

void lift_pump(void)
{
    pump_motor_gpos = HIGHEST_POSITION;
    set_goal_position(dxl_port_num, MOTOR_3_ID, pump_motor_gpos); 
}


void pick_up_bar(bool is_double)
{
    gpio_set_level(PUMP_PIN, true);

    move_pump_motor((read_present_position(dxl_port_num, MOTOR_3_ID) - (is_double ? BAR_TOLERANCE_DOUBLE : BAR_TOLERANCE_SINGLE)), 80, MOTOR_3_ID); // lower the handle
    
    vTaskDelay(1000 / portTICK_PERIOD_MS); 

    move_pump_motor(HIGHEST_POSITION, 20, MOTOR_3_ID); // move the handle up

}

void release_bar(void)
{
    gpio_set_level(PUMP_PIN, false); 
}

void rotate_and_release_bar(double angle_deg)
{
    double arc_mm = WHEELS_DISTANCE_mm * (angle_deg * M_PI / 360);   
    
    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_while_dropping_bar(sw_group_nums[2], arc_mm, -arc_mm);

    profile_vel_sw[0] = MAX_VEL / 5 - 1;
    profile_vel_sw[1] = MAX_VEL / 5 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    vTaskDelay(500 / portTICK_PERIOD_MS); 
}