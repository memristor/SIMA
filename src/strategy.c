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
    set_goal_position(dxl_port_num, MOTOR_3_ID, 3000);
    printf("POS 3 READ 1: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_3_ID));

    move_motors_mm(sw_group_nums[2], 1000, 1000);
    rotate_motors(90);
    set_goal_position(dxl_port_num, MOTOR_3_ID, 3300);
    printf("POS 3 READ 2: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_3_ID));

    move_motors_mm(sw_group_nums[2], 10000, 10000);
    */
    /* ovo je test kupljenja
    move_motors_mm(sw_group_nums[2], -400, -400);
    pick_up_bar();
    rotate_pump(72);
    vTaskDelay(1500/portTICK_PERIOD_MS);
    rotate_pump(-60);
    move_motors_mm(sw_group_nums[2], 400, 400);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    release_bar();
    */

    //test guranja dasaka sa podijuma
    move_motors_mm(sw_group_nums[2],-250, -250);
    move_motors_mm(sw_group_nums[2],190, 190);
    rotate_motors(-50, false);
    move_motors_mm(sw_group_nums[2],-360, -360);



}

void SIMA_N_BLUE()
{
    //kupljenje dve daske, ostavljanje i guranje crnih i plavih
    rotate_motors(15, false);
    move_motors_mm(sw_group_nums[2],160, 160);
    rotate_motors(-15, false);
    move_motors_mm(sw_group_nums[2],262, 262);
    pick_up_bar();
    rotate_pump(90);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_pump(-90);
    //razlika u pumpi od pocetne pozicije -18
    vTaskDelay(1000/portTICK_PERIOD_MS);
    move_motors_mm(sw_group_nums[2],-50, -50);
    rotate_motors(-15, true);
    move_motors_mm(sw_group_nums[2],-65, -65);
    rotate_motors(15, true);
    move_motors_mm(sw_group_nums[2],-285, -285);
    rotate_motors(90, true);
    vTaskDelay(500/portTICK_PERIOD_MS);
    release_bar();
    move_motors_mm(sw_group_nums[2], -20, -20);
    vTaskDelay(500/portTICK_PERIOD_MS);
    rotate_motors(-90, false);
    vTaskDelay(500/portTICK_PERIOD_MS);
    //zavrsio ostavljanje prve daske
    move_motors_mm(sw_group_nums[2],470, 470);
    pick_up_bar();
    rotate_pump(90);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_pump(-90);
    //razlika u pumpi od pocetne pozicije 0
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_motors(15, true);
    move_motors_mm(sw_group_nums[2],-50, -50);
    rotate_motors(-15, true);
    move_motors_mm(sw_group_nums[2],-400, -400);
    rotate_motors(90, true);
    release_bar();
    rotate_motors(-75, false);
    move_motors_mm(sw_group_nums[2],650, 650);
    rotate_motors(6.5, false);
    vTaskDelay(500/portTICK_PERIOD_MS);
    move_motors_mm(sw_group_nums[2],-610, -610);
    rotate_motors(-100, false);
}


