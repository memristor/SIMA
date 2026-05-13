#include "stdio.h"
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "timer.h" 
#include "cinc_logic.h"
#include "strategy.h" 
#include "sensor.h"
#include "servo.h"
#include "init.h"
#include "pump.h"
#include "debug.h"

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

    // prep pump
    setup_pump();

   
    if(DEBUG_ENABLED){
        wifi_init();
        xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
    }
    
    if (!read_position(group_num_sr, present_pos_read))
    {
        print("Position read failed.\n");
    }
    else
    {
        print("Position read successful!\n");
        print("Motor 1 position: %ld\n", present_pos_read[0]);
        print("Motor 2 position: %ld\n", present_pos_read[1]);
    }

    print("GROUP NUM SR: %d\n", group_num_sr);    
    print("ACC: %d\n", sw_group_nums[0]);
    print("VEL: %d\n", sw_group_nums[1]);
    print("POS: %d\n", sw_group_nums[2]);

    if(strategy == HOMOLOGACIJA){
        create_check_sensors_task();
    }
    
    while (1)
    {
        print("Waiting for CINC to get pulled!\n\r");

        check_led();
        
        if (prev_cinc && cinc && !timer_on)
        {
            print("CINC pulled, starting strat!\n\r");
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

            check_strat();

            motors_moving = true;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
        
    }   
}