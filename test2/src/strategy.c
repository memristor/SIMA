#include "strategy.h"
#include "servo.h"
#include "motor_logic.h"

#include "freertos/FreeRTOS.h"
#include "FreeRTOSConfig.h"

TaskHandle_t check_flag_handle;
TaskHandle_t stop_motors_handle;

//Check SE FLAG SE MOZE IZBACITI A DA SE U MAIN UBACI POKRETANJE SERVA NA END_FLAG?
// continuous task
void move_servo_task(void *pvParams)
{
    //BaseType_t freeSpace = uxTaskGetStackHighWaterMark(check_flag_handle);

    //const TickType_t xFreq = pdMS_TO_TICKS(20);

    //TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        //xTaskDelayUntil(&xLastWakeTime, xFreq);
        //printf("CHECK TASK\n");

        //printf("Free space left: %d\n", freeSpace);

        /*printf("Task is running on core: ");
        printf("%d\n", xPortGetCoreID());*/

        move_motor();
    }
}

void stop_motors_end(void *pvParams)
{
    const TickType_t xFreq = pdMS_TO_TICKS(10);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        xTaskDelayUntil(&xLastWakeTime, xFreq);
        //printf("STOP TASK\n");

        //vTaskDelay(10 / portTICK_PERIOD_MS);
        if (end_flag == true)
        {
            shut_motors_off();
            create_move_servo_task();
            vTaskSuspend(NULL);
        }
    }
    
}

void create_move_servo_task()
{
    BaseType_t creation_result;

    creation_result = xTaskCreatePinnedToCore(
                                            move_servo_task,
                                            "MOVE_SERVO_TASK",
                                            4096,
                                            NULL,
                                            2,                 
                                            &check_flag_handle,
                                            1                       // Stavljeno na CORE1 sa tajmerom
                                            );                      // (izmedju tajmera koji je evenet task je provera)

    if (creation_result != pdPASS)
    {
        printf("Move servo task creation error.\n");
    }
    else
    {
        printf("Move servo task created.\n");
    }
}

void create_stop_motors_end_task()
{
    BaseType_t creation_result;

    creation_result = xTaskCreatePinnedToCore(
                                            stop_motors_end,
                                            "STOP_MOTORS_END",
                                            4096,
                                            NULL,
                                            2,
                                            &stop_motors_handle,
                                            1
                                            );

    if (creation_result != pdPASS)
    {
        printf("Stop motors end task creation error.\n");
    }
    else
    {
        printf("Stop motors end task created.\n");
    }
}

void shut_motors_off()
{
    goal_pos_sw[0] = present_pos_read[0];
    goal_pos_sw[1] = present_pos_read[1];
    sync_write_gposition(sw_group_nums[2], goal_pos_sw);

    //profile_vel_sw[0] = MIN_VEL_ACC;
    //profile_vel_sw[1] = MIN_VEL_ACC;
    //sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    //set_torque_enable(dxl_port_num, MOTOR_1_ID, false);
    //vTaskDelay(10 / portTICK_PERIOD_MS);
    //set_torque_enable(dxl_port_num, MOTOR_2_ID, false);
    //vTaskDelay(10 / portTICK_PERIOD_MS);
}

void SIMA_N_YELLOW()
{
    move_motors_mm(sw_group_nums[2], 700, 700);
    rotate_motors(10);
    move_motors_mm(sw_group_nums[2], 100, 100);
    rotate_motors(35);
    move_motors_mm(sw_group_nums[2], 540, 540);
    rotate_motors(-45);
    move_motors_mm(sw_group_nums[2], 600, 600);
    rotate_motors(90);
    move_motors_mm(sw_group_nums[2], 150, 150);
}

void SIMA_N_BLUE()
{
    move_motors_mm(sw_group_nums[2], 700, 700);
    rotate_motors(-10);
    move_motors_mm(sw_group_nums[2], 100, 100);
    rotate_motors(-35);
    move_motors_mm(sw_group_nums[2], 540, 540);
    rotate_motors(45);
    move_motors_mm(sw_group_nums[2], 600, 600);
    rotate_motors(-90);
    move_motors_mm(sw_group_nums[2], 150, 150);
}


