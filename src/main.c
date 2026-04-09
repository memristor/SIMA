#include "stdio.h"
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "motor_logic.h"
#include "timer.h" 
#include "cinc_logic.h"
#include "strategy.h" 
#include "sensor.h"
#include "servo.h"
#include "init.h"
#include "pump.h"

bool motors_enabled = false;
bool motors_moving = false;

void app_main() 
{

    init_strat();
    setup_sync();
    setupMotors();
    sensor_init();
    setup_servo();
    init_timer();
    
    if (!read_position(group_num_sr, present_pos_read))
    {
        printf("Position read failed.\n");
    }
    else
    {
        printf("Position read successful!\n");
        printf("Motor 1 position: %ld\n", present_pos_read[0]);
        printf("Motor 2 position: %ld\n", present_pos_read[1]);
    }

    printf("GROUP NUM SR: %d\n", group_num_sr);    
    printf("ACC: %d\n", sw_group_nums[0]);
    printf("VEL: %d\n", sw_group_nums[1]);
    printf("POS: %d\n", sw_group_nums[2]);

    profile_vel_sw[0] = MAX_VEL_ACC;
    profile_vel_sw[1] = MAX_VEL_ACC;

    sync_write_velocity(sw_group_nums[1], profile_vel_sw);
    set_profile_velocity(dxl_port_num, MOTOR_3_ID, MAX_VEL_ACC/4);
   
    profile_acc_sw[0] = MAX_VEL_ACC/20;
    profile_acc_sw[1] = MAX_VEL_ACC/20;

    sync_write_acceleration(sw_group_nums[0], profile_acc_sw);
    set_profile_acceleration(dxl_port_num, MOTOR_3_ID, MAX_VEL_ACC/20);
    setup_pump();

    
    pick_up_bar();
    rotate_pump(90);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    rotate_pump(-90);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    rotate_motors(90, true);
    //move_motors_mm(sw_group_nums[2], -1100, -1100);
    /*
    rotate_pump(90);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    release_bar();
    */
    //rotate_motors(90);
    
    while (1)
    {
        /*
        check_led();
        
        if (prev_cinc && cinc && !timer_on)
        {
            //printf("CINC pulled: Sending ENABLE signal");
            motors_enabled = true;
        }
        prev_cinc = cinc;
        

        if (motors_enabled == true)
        {
            // Ovde bi se pozvao tajmer i proverila strategija po
            // kojoj bi se SIMA kretala
            start_timer();
            motors_enabled = false;
        } 

        if (start_flag == true && motors_moving == false)
        {
            create_stop_motors_end_task();
            create_check_sensors_task();

            check_strat();

            motors_moving = true;
        }*/

        vTaskDelay(20 / portTICK_PERIOD_MS);
        
    }   
}