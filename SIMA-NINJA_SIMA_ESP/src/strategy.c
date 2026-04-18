#include "strategy.h"
#include "servo.h"
#include "motor_logic.h"
#include "pump.h"

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
    /*
    //ovo je kod za guranje - STO NA KOM JE VELIKI PROKLIZAVAO (onaj dalje od naseg ormana)
    // |
    // |
    // V
    move_motors_mm(sw_group_nums[2], 350, 350);
    rotate_motors(-25, false);
    move_motors_mm(sw_group_nums[2], 50, 50);
    rotate_motors(25, false);
    move_motors_mm(sw_group_nums[2], 100, 100);
    rotate_motors(-35, false);
    move_motors_mm(sw_group_nums[2], 200, 200);
    move_motors_mm(sw_group_nums[2], -30, -30);
    // vTaskDelay(500/portTICK_PERIOD_MS);
    rotate_motors(45, false);
    move_motors_mm(sw_group_nums[2], -260, -260);
    move_motors_mm(sw_group_nums[2], 80, 80);
    rotate_motors(79, false);
    move_motors_mm(sw_group_nums[2], 159, 159);
    rotate_motors(-90, false);
    move_motors_mm(sw_group_nums[2], -405, -405);
    rotate_motors(88, false);

    // vTaskDelay(57000/portTICK_PERIOD_MS);
    vTaskDelay(55000/portTICK_PERIOD_MS);
    
    move_motors_mm(sw_group_nums[2], -251, -251);
    move_motors_mm(sw_group_nums[2], 210, 210);
    rotate_motors(89, false);
    move_motors_mm(sw_group_nums[2], -430, -430);
    rotate_motors(-90, false);
    move_motors_mm(sw_group_nums[2], -235, -235);
    */

    //safe opcija za NINJU
    move_motors_mm(sw_group_nums[2], -110, -110);
    rotate_motors(-90, false);

    //za non-aggressive
    vTaskDelay(60000/portTICK_PERIOD_MS);

    //za metlicu
    //vTaskDelay(68000/portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -265, -265);
    move_motors_mm(sw_group_nums[2], 65, 65);
    rotate_motors(90, false);
    move_motors_mm(sw_group_nums[2], -490, -490);
    move_motors_mm(sw_group_nums[2], 70, 70);
    rotate_motors(-90, false);
    move_motors_mm(sw_group_nums[2], -85, -85);

    //linije ispod move 65, 65 zakomentarisati
    //i ako igramo protiv +381 ili nts narandzastih sima 
    //otkomentarisati sledece:
    
    /*
    rotate_motors(73, false);
    move_motors_mm(sw_group_nums[2], -450, -450)
    */


}

void SIMA_N_BLUE()
{   
    /*
    //ovo je kod za guranje - STO NA KOM JE VELIKI PROKLIZAVAO (onaj dalje od naseg ormana)
    // |
    // |
    // V
    move_motors_mm(sw_group_nums[2], 350, 350);
    rotate_motors(25, false);
    move_motors_mm(sw_group_nums[2], 50, 50);
    rotate_motors(-25, false);
    move_motors_mm(sw_group_nums[2], 100, 100);
    rotate_motors(35, false);
    move_motors_mm(sw_group_nums[2], 200, 200);
    move_motors_mm(sw_group_nums[2], -30, -30);

    // vTaskDelay(500/portTICK_PERIOD_MS);
    
    rotate_motors(-45, false);
    move_motors_mm(sw_group_nums[2], -260, -260);
    move_motors_mm(sw_group_nums[2], 80, 80);
    rotate_motors(-79, false);
    move_motors_mm(sw_group_nums[2], 159, 159);
    rotate_motors(88, false);
    move_motors_mm(sw_group_nums[2], -405, -405);
    rotate_motors(-86, false);

    // vTaskDelay(57000/portTICK_PERIOD_MS);
    vTaskDelay(55000/portTICK_PERIOD_MS);
    
    move_motors_mm(sw_group_nums[2], -251, -251);
    move_motors_mm(sw_group_nums[2], 210, 210);
    rotate_motors(-89, false);
    move_motors_mm(sw_group_nums[2], -430, -430);
    rotate_motors(90, false);
    move_motors_mm(sw_group_nums[2], -235, -235);
    */

    //safe opcija za NINJU
    move_motors_mm(sw_group_nums[2], -110, -110);
    rotate_motors(90, false);

    //za non-aggressive
    vTaskDelay(60000/portTICK_PERIOD_MS);

    //za metlicu
    //vTaskDelay(68000/portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -265, -265);
    move_motors_mm(sw_group_nums[2], 65, 65);
    rotate_motors(-90, false);
    move_motors_mm(sw_group_nums[2], -490, -490);
    move_motors_mm(sw_group_nums[2], 70, 70);
    rotate_motors(90, false);
    move_motors_mm(sw_group_nums[2], -85, -85);

    //linije ispod move 65, 65 zakomentarisati
    //i ako igramo protiv +381 ili nts narandzastih sima 
    //otkomentarisati sledece:
    
    /*
    rotate_motors(73, false);
    move_motors_mm(sw_group_nums[2], -450, -450)
    */


}


