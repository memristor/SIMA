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
bool motors_reset = false;

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

    profile_vel_sw[0] = 240;
    profile_vel_sw[1] = 240;

    sync_write_velocity(sw_group_nums[1], profile_vel_sw);
   
    profile_acc_sw[0] = 500;
    profile_acc_sw[1] = 500;

    sync_write_acceleration(sw_group_nums[0], profile_acc_sw);

    //start_timer();

    //vTaskDelay(10000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], 1000, 1000);

    //vTaskDelay(10000 / portTICK_PERIOD_MS);

    //rotate_motors(90);

    //move_motors_mm(sw_group_nums[2], 200, 200);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    //profile_vel_sw[0] = MAX_VEL_ACC;
    //profile_vel_sw[1] = MAX_VEL_ACC;

    //sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    start_timer();
    create_stop_motors_end_task();
    create_check_sensors_task();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], 10000, 10000);

    //goal_pos_sw[0] = -100000;
    //goal_pos_sw[1] =  100000;
    
    //sync_write_gposition(sw_group_nums[2], goal_pos_sw);
    
    //set_operating_mode(dxl_port_num, MOTOR_1_ID, OP_POSITION);

    //set_control_table(dxl_port_num, MOTOR_1_ID, VELOCITY_LIM_ADDR, 265, 4, 1);
    //set_control_table(dxl_port_num, MOTOR_2_ID, VELOCITY_LIM_ADDR, 265, 4, 1);

    //printf("Velocity limit read 1: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_1_ID));
    //printf("Velocity limit read 2: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_2_ID));

    while (1)
    {
        printf("Samo da ne puca\n");

        //vTaskDelay(5000 / portTICK_PERIOD_MS);

        check_led();

        if (!prev_cinc && !cinc)
        {
            printf("CINC pulled: Sending ENABLE signal");
            motors_enabled = true;
        }
        prev_cinc = cinc;

        if (motors_enabled == true)
        {
            // Ovde bi se pozvao tajmer i proverila strategija po
            // kojoj bi se SIMA kretala
            motors_enabled = false;
        }
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /*
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

        vTaskDelay(5000 / portTICK_PERIOD_MS);
        broadcastPing(dxl_port_num, DXL_PROTOCOL);

        printf("Broadcast ping test: \n");
        if (packetData[dxl_port_num].communication_result != COMM_SUCCESS)
        {
            printf("PING FAIL! IDE DIZEL :(\n");
            printf("%s\n", getTxRxResult(DXL_PROTOCOL, packetData[dxl_port_num].communication_result));
            printf("%s\n", getRxPacketError(DXL_PROTOCOL, packetData[dxl_port_num].error));
        }
        else
        {
            printf("PING SUCCESSFUL! IDE GASSSS\n");
        }

        printf("Detected IDs from broadcast: ");
        for (int id = 1; id <= 10; id++) {  // Check common IDs
            if (getBroadcastPingResult(dxl_port_num, DXL_PROTOCOL, id)) {
                printf("%d ", id);
            }
        }
        printf("\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);


        ping(dxl_port_num, DXL_PROTOCOL, MOTOR_1_ID);

        printf("Regular ping test 1: \n");
        if (packetData[dxl_port_num].communication_result != COMM_SUCCESS)
        {
            printf("PING FAIL! IDE DIZEL :(\n");
            printf("%s\n", getTxRxResult(DXL_PROTOCOL, packetData[dxl_port_num].communication_result));
            printf("%s\n", getRxPacketError(DXL_PROTOCOL, packetData[dxl_port_num].error));
        }
        else
        {
            printf("PING SUCCESSFUL! IDE GASSSS\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        ping(dxl_port_num, DXL_PROTOCOL, MOTOR_2_ID);

        printf("Regular ping test 2: \n");
        if (packetData[dxl_port_num].communication_result != COMM_SUCCESS)
        {
            printf("PING FAIL! IDE DIZEL :(\n");
            printf("%s\n", getTxRxResult(DXL_PROTOCOL, packetData[dxl_port_num].communication_result));
            printf("%s\n", getRxPacketError(DXL_PROTOCOL, packetData[dxl_port_num].error));
        }
        else
        {
            printf("PING SUCCESSFUL! IDE GASSSS\n");
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
        clearPort(dxl_port_num);
        printf("\n"); 
        */

    }
    
}