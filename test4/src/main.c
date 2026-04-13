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

bool motors_enabled = false;
bool motors_moving = false;

void app_main() 
{
    //printf("UART/DXL port name: %s\n", getPortName(dxl_port_num));
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
    //printf("UART/DXL set baudrate: %d\n", getBaudRate(dxl_port_num));

    /*if (uart_get_baudrate(dxl_port_num, &esp_baudrate) != ESP_FAIL)
    {
        printf("Baudrate read successful: %ld\n", esp_baudrate);
    }
    else
    {
        printf("Baudrate read failed!\n");  
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("Address of port in packetData: %p\n", packetData);*/

    init_strat();
    setup_sync();
    setupMotors();
    sensor_init();
    setup_servo();
    init_timer();

    //printf("Velocity limit read 1: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_1_ID));
    //printf("Velocity limit read 2: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_2_ID));
    
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
    profile_vel_sw[1] = MAX_VEL_ACC - 2;

    sync_write_velocity(sw_group_nums[1], profile_vel_sw);
   
    profile_acc_sw[0] = MAX_VEL_ACC / 10;
    profile_acc_sw[1] = MAX_VEL_ACC / 10;

    sync_write_acceleration(sw_group_nums[0], profile_acc_sw);
    //move_motors_mm(sw_group_nums[2], 1000, 1000);

    while (1)
    {
        
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
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            create_check_sensors_task();

            check_strat();

            motors_moving = true;
        }
        
        vTaskDelay(20 / portTICK_PERIOD_MS);

    }
    
}