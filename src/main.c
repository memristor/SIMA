#include "stdio.h"
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "motor_logic.h"


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

    setupMotors();
    printf("Velocity limit read 1: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_1_ID));
    printf("Velocity limit read 2: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_2_ID));
    int group_num = setup_sync_read(PRESENT_POS_ADDR, 4);

    uint32_t present_pos_read[2] = {0, 0};

    if (!read_position(group_num, present_pos_read))
    {
        printf("Position read failed.\n");
    }
    else
    {
        printf("Position read successful!\n");
        printf("Motor 1 position: %ld\n", present_pos_read[0]);
        printf("Motor 2 position: %ld\n", present_pos_read[1]);
    }

    int sw_group_nums[3] = {0, 0, 0};

    setup_sw(sw_group_nums);

    profile_vel_sw[0] = 120;
    profile_vel_sw[1] = 120;

    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    printf("Profile velocity 1: %ld\n", read_profile_velocity(dxl_port_num, MOTOR_1_ID));
    printf("Profile velocity 2: %ld\n", read_profile_velocity(dxl_port_num, MOTOR_2_ID));

    profile_acc_sw[0] = 240;
    profile_acc_sw[1] = 240;

    sync_write_acceleration(sw_group_nums[0], profile_acc_sw);

    printf("Profile acceleration 1: %ld\n", read_profile_acceleration(dxl_port_num, MOTOR_1_ID));
    printf("Profile acceleration 2: %ld\n", read_profile_acceleration(dxl_port_num, MOTOR_2_ID));

    goal_pos_sw[0] = 1000;
    goal_pos_sw[1] = 1000;

    sync_write_gposition(sw_group_nums[2], goal_pos_sw);

    printf("Goal position 1: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_1_ID));
    printf("Goal position 2: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_2_ID));

    //set_operating_mode(dxl_port_num, MOTOR_1_ID, OP_POSITION);

    //set_control_table(dxl_port_num, MOTOR_1_ID, VELOCITY_LIM_ADDR, 265, 4, 1);
    //set_control_table(dxl_port_num, MOTOR_2_ID, VELOCITY_LIM_ADDR, 265, 4, 1);

    //printf("Velocity limit read 1: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_1_ID));
    //printf("Velocity limit read 2: %ld\n", read_velocity_limit(dxl_port_num, MOTOR_2_ID));

    while (1)
    {
        printf("Samo da ne puca\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        /*broadcastPing(dxl_port_num, DXL_PROTOCOL);

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