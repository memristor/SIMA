#include "strategy.h"
#include "sensor.h"
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
        printf("TestTestTest\n");
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
    sync_write_gposition(sw_group_nums[2], present_pos_read);
}

void SIMA_N_YELLOW()
{
    move_motors_mm(sw_group_nums[2], 250, 250);
    rotate_motors(48.81);
    move_motors_mm(sw_group_nums[2], 532.1, 532.1);
    rotate_motors(42.19);
    move_motors_mm(sw_group_nums[2], 650, 650);
    rotate_motors(-45);
    move_motors_mm(sw_group_nums[2], 192.32, 192.32);

    // izgurivanje protivniku uz platformu
    // ovo nakon move 650
    //rotate_motors(40.6);
    //move_motors_mm(sw_group_nums[2], 422.98, 422.98);
    //rotate_motors(49.4);
    //move_motors_mm(sw_group_nums[2], 75, 75);
    //rotate_motors(-90);
    //move_motors_mm(sw_group_nums[2], 270.16, 270.16);

}

void SIMA_N_BLUE()
{
    
    move_motors_mm(sw_group_nums[2], 250, 250);
    rotate_motors(-48.81);
    move_motors_mm(sw_group_nums[2], 532.1, 532.1);
    rotate_motors(-42.19);
    move_motors_mm(sw_group_nums[2], 650, 650);
    rotate_motors(45);
    move_motors_mm(sw_group_nums[2], 192.32, 192.32);

    // izgurivanje protivniku uz platformu
    // ovo nakon move 650
    //rotate_motors(-40.6);
    //move_motors_mm(sw_group_nums[2], 422.98, 422.98);
    //rotate_motors(-49.4);
    //move_motors_mm(sw_group_nums[2], 75, 75);
    //rotate_motors(90);
    //move_motors_mm(sw_group_nums[2], 270.16, 270.16);

}


