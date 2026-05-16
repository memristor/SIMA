#include "strategy.h"
#include "servo.h"
#include "motor_logic.h"
#include "pump.h"

#include "freertos/FreeRTOS.h"
#include "FreeRTOSConfig.h"

TaskHandle_t check_flag_handle;
TaskHandle_t stop_motors_handle;

// continuous task
void move_servo_task(void *pvParams)
{
    while (true)
    {
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
                                            );                      // (izmedju tajmera koji je event task je provera)

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
}

void SIMA_N_HOMOLOGACIJA()
{
    move_motors_mm(sw_group_nums[2], -400, -400);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    disable_sensors = true;

    lift_pump();

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    prep_pump();

    move_motors_mm(sw_group_nums[2], -50, -50);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);
}

void SIMA_N_TEST_ROTATION(void)
{
    rotate_motors(90, false);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    rotate_motors(-90, false);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    rotate_motors(45, false);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    rotate_motors(-45, false);
}

void SIMA_N_YELLOW_NO_BARS_SAFE_PUSH_FIRST() // zavrseno
{

    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(62000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -50, -50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 70, 70);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -540, -540);

    move_motors_mm(sw_group_nums[2], 160, 160);

    rotate_motors(-90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -40, -40);

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -55, -55);

}

void SIMA_N_BLUE_NO_BARS_SAFE_PUSH_FIRST() // zavrseno
{
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(62000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -50, -50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 70, 70);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -540, -540);

    move_motors_mm(sw_group_nums[2], 160, 160);

    rotate_motors(90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -40, -40);

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -55, -55);

}

void SIMA_N_BLUE_NO_BARS_SAFE()
{
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 80, 80);

    rotate_motors(-60, false);

    move_motors_mm(sw_group_nums[2], -140, -140);

    rotate_motors(-30, false);

    move_motors_mm(sw_group_nums[2], -450, -450);

    move_motors_mm(sw_group_nums[2], 360, 360);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -370, -370);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 110, 110);

    move_motors_mm(sw_group_nums[2], -140, -140);

    vTaskDelay(47000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -120, -120);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -355, -355);

    rotate_motors(90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -60, -60);

    vTaskDelay(17000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

}


void SIMA_N_YELLOW_NO_BARS_SAFE()
{
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 80, 80);

    rotate_motors(60, false);

    move_motors_mm(sw_group_nums[2], -140, -140);

    rotate_motors(30, false);

    move_motors_mm(sw_group_nums[2], -450, -450);

    move_motors_mm(sw_group_nums[2], 360, 360);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -370, -370);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 110, 110);

    move_motors_mm(sw_group_nums[2], -140, -140);

    vTaskDelay(47000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -120, -120);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -355, -355);

    rotate_motors(-90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -60, -60);

    vTaskDelay(17000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

}



void SIMA_N_YELLOW_NO_BARS_AGGRESSIVE()
{
    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 3  - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -400, -410);

    profile_vel_sw[0] = MAX_VEL / 2.2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -680, -680);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -110, -150);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 130, 200);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 330, 330);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -820, -820);

    move_motors_mm(sw_group_nums[2], 100, 100);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 105, 105);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(44000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -70, -70);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 100, 100);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);
}


void SIMA_N_BLUE_NO_BARS_AGGRESSIVE()
{
    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -410, -400);

    profile_vel_sw[0] = MAX_VEL / 2.2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -680, -680);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -150, -110);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 200, 130);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 330, 330);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -820, -820);

    move_motors_mm(sw_group_nums[2], 100, 100);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 105, 105);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(44000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -70, -70);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 100, 100);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);
}

void SIMA_N_YELLOW_NO_BARS_AGGRESSIVE_TEAMAUTO()
{
    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 3  - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -400, -410);

    profile_vel_sw[0] = MAX_VEL / 2.2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -400, -400);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -100, -140);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 130, 200);

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -300, -300);


    move_motors_mm(sw_group_nums[2], 630, 630);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -820, -820);

    move_motors_mm(sw_group_nums[2], 100, 100);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 105, 105);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(34000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -70, -70);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 100, 100);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);
}


void SIMA_N_BLUE_NO_BARS_AGGRESSIVE_TEAMAUTO()
{
    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -410, -400);

    profile_vel_sw[0] = MAX_VEL / 2.2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -400, -400);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -100, -140);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 140, 100);

    vTaskDelay(14000 / portTICK_PERIOD_MS);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -300, -300);

    move_motors_mm(sw_group_nums[2], 630, 630);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -820, -820);

    move_motors_mm(sw_group_nums[2], 100, 100);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 105, 105);

    move_motors_mm(sw_group_nums[2], -110, -110);

    vTaskDelay(44000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -140, -140);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -70, -70);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 100, 100);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);
}

void SIMA_N_BLUE_NO_BARS_AGGRESSIVE_GRCI()
{
    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -1070, -1070);

    rotate_motors(-5, false);

    move_motors_mm(sw_group_nums[2], 400, 400);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -670, -670);

    move_motors_mm(sw_group_nums[2], 100, 100);    

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 120, 120);

    move_motors_mm(sw_group_nums[2], -140, -140);
    
    vTaskDelay(52000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);  

    move_motors_mm(sw_group_nums[2], -120, -120);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -355, -355);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 230, 230);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(16000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);

}

void SIMA_N_YELLOW_NO_BARS_AGGRESSIVE_GRCI()
{
    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL / 2.2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    lift_pump();

    move_motors_mm(sw_group_nums[2], -1070, -1070);

    rotate_motors(5, false);

    move_motors_mm(sw_group_nums[2], 400, 400);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 340, 340);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -670, -670);

    move_motors_mm(sw_group_nums[2], 100, 100);    

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 120, 120);

    move_motors_mm(sw_group_nums[2], -140, -140);
    
    vTaskDelay(52000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);  

    move_motors_mm(sw_group_nums[2], -120, -120);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 110, 110);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -355, -355);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 230, 230);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(16000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -100, -100);

}


void SIMA_N_YELLOW2()
{ 
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 60, 60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    prep_pump();

    move_motors_mm(sw_group_nums[2], -270, -270);

    rotate_motors(-45, false);

    move_motors_mm(sw_group_nums[2], -80, -80);

    rotate_motors(5, false);

    move_motors_mm(sw_group_nums[2], -50, -50);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -360, -360);

    // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], -60, -60);

    //////////////////////////////////////////////////////

    move_motors_mm(sw_group_nums[2], 250, 250);

    rotate_motors(90, false);

    prep_pump();

    move_motors_mm(sw_group_nums[2], 200, 200);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(45, false);

    move_motors_mm(sw_group_nums[2], -230, -230);

    rotate_motors(15, false);

    move_motors_mm(sw_group_nums[2], -20, -20);

    move_motors_mm(sw_group_nums[2], 10, 10);

    pick_up_bar(true);

    rotate_motors(-55, false);

    move_motors_mm(sw_group_nums[2], 290, 290);

    move_motors_mm(sw_group_nums[2], -70, -70);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -500, -500);

     // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(-50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(50, false);

    move_motors_mm(sw_group_nums[2], -40, -40);

    move_motors_mm(sw_group_nums[2], 300, 300);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    move_motors_mm(sw_group_nums[2], -170, -170);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -370, -370);

    rotate_motors(-5, false);

    move_motors_mm(sw_group_nums[2], 555, 555);

    rotate_motors(-85, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    vTaskDelay(12000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -260, -260);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);


    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -340, -340);

    rotate_motors(-90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(15000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -45, -45);
    //////////////////////////

}

void SIMA_N_BLUE2()
{ 
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 60, 60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    prep_pump();

    move_motors_mm(sw_group_nums[2], -270, -270);

    rotate_motors(45, false);

    move_motors_mm(sw_group_nums[2], -80, -80);

    rotate_motors(-5, false);

    move_motors_mm(sw_group_nums[2], -50, -50);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(50, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(-50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(50, false);

    move_motors_mm(sw_group_nums[2], -60, -60);

    //////////////////////////////////////////////////////

    move_motors_mm(sw_group_nums[2], 250, 250);

    rotate_motors(-90, false);

    prep_pump();

    move_motors_mm(sw_group_nums[2], 200, 200);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-45, false);

    move_motors_mm(sw_group_nums[2], -230, -230);

    rotate_motors(-15, false);

    move_motors_mm(sw_group_nums[2], -20, -20);

    move_motors_mm(sw_group_nums[2], 10, 10);

    pick_up_bar(true);

    rotate_motors(55, false);

    move_motors_mm(sw_group_nums[2], 290, 290);

    move_motors_mm(sw_group_nums[2], -70, -70);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -500, -500);

     // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], -40, -40);

    move_motors_mm(sw_group_nums[2], 300, 300);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    move_motors_mm(sw_group_nums[2], -170, -170);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -370, -370);

    rotate_motors(5, false);

    move_motors_mm(sw_group_nums[2], 555, 555);

    rotate_motors(85, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    vTaskDelay(12000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -260, -260);

    profile_vel_sw[0] = MAX_VEL / 2 - 1;
    profile_vel_sw[1] = MAX_VEL / 2 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -60, -60);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 310, 310);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -340, -340);

    rotate_motors(90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(15000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -45, -45);
    //////////////////////////

}



void SIMA_N_YELLOW4()
{ 
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 60, 60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    prep_pump();

    move_motors_mm(sw_group_nums[2], -270, -270);

    rotate_motors(-45, false);

    move_motors_mm(sw_group_nums[2], -80, -80);

    rotate_motors(5, false);

    move_motors_mm(sw_group_nums[2], -50, -50);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-91, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], -60, -60);

    //////////////////////////////////////////////////////

    move_motors_mm(sw_group_nums[2], 250, 250);

    rotate_motors(90, false);

    prep_pump();

    move_motors_mm(sw_group_nums[2], 200, 200);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(45, false);

    move_motors_mm(sw_group_nums[2], -230, -230);

    rotate_motors(15, false);

    move_motors_mm(sw_group_nums[2], -20, -20);

    move_motors_mm(sw_group_nums[2], 10, 10);

    pick_up_bar(true);

    rotate_motors(-55, false);

    move_motors_mm(sw_group_nums[2], 290, 290);

    move_motors_mm(sw_group_nums[2], -70, -70);

    rotate_motors(-87, false);

    move_motors_mm(sw_group_nums[2], -500, -500);

     // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(-50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(50, false);

    move_motors_mm(sw_group_nums[2], -40, -40);

    move_motors_mm(sw_group_nums[2], 300, 300);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    move_motors_mm(sw_group_nums[2], -170, -170);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -370, -370);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-91, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-50, false);

    move_motors_mm(sw_group_nums[2], -60, -60);
    rotate_motors(-5, false);

    move_motors_mm(sw_group_nums[2], 555, 555);

    rotate_motors(-85, false);

    move_motors_mm(sw_group_nums[2], 150, 150);

    move_motors_mm(sw_group_nums[2], -260, -260);

    profile_vel_sw[0] = MAX_VEL - 1;
    profile_vel_sw[1] = MAX_VEL - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);


    move_motors_mm(sw_group_nums[2], -50, -50);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -340, -340);

    rotate_motors(-90, false);

    starting_pos();

    move_motors_mm(sw_group_nums[2], -200, -200);

    vTaskDelay(12000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -60, -60);
    //////////////////////////

}



void SIMA_N_BLUE4()
{
    lift_pump();

    move_motors_mm(sw_group_nums[2], -130, -130);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], 130, 130);

    move_motors_mm(sw_group_nums[2], -175, -175);

    move_motors_mm(sw_group_nums[2], 60, 60);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 190, 190);

    prep_pump();

    move_motors_mm(sw_group_nums[2], -250, -250);

    rotate_motors(45, false);

    move_motors_mm(sw_group_nums[2], -50, -50);

    rotate_motors(-15, false);

    move_motors_mm(sw_group_nums[2], -50, -50);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(65, false);

    move_motors_mm(sw_group_nums[2], 300, 300);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(95, false);

    move_motors_mm(sw_group_nums[2], -350, -350);

    // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL / 3 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(-30);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(30, false);

    move_motors_mm(sw_group_nums[2], -60, -60);

    //////////////////////////////////////////////////////

    move_motors_mm(sw_group_nums[2], 250, 250);

    rotate_motors(-90, false);

    prep_pump();

    move_motors_mm(sw_group_nums[2], 200, 200);

    move_motors_mm(sw_group_nums[2], -60, -60);

    rotate_motors(-45, false);

    move_motors_mm(sw_group_nums[2], -220, -220);

    rotate_motors(-10, false);

    move_motors_mm(sw_group_nums[2], -20, -20);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    rotate_motors(65, false);

    move_motors_mm(sw_group_nums[2], 290, 290);

    move_motors_mm(sw_group_nums[2], -70, -70);

    rotate_motors(95, false);

    move_motors_mm(sw_group_nums[2], -500, -500);

     // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL / 3 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(30);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-30, false);

    move_motors_mm(sw_group_nums[2], -60, -60);

    //////////////////////////////////////////////////////

    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL / 3 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], 380, 380);

    rotate_motors(-90, false);

    prep_pump();

    move_motors_mm(sw_group_nums[2], 230, 230);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    move_motors_mm(sw_group_nums[2], -180, -180);

    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], -270, -270);

    move_motors_mm(sw_group_nums[2], 15, 15);

    pick_up_bar(true);

    move_motors_mm(sw_group_nums[2], 280, 280);

    rotate_motors(100, false);

    move_motors_mm(sw_group_nums[2], 180, 180);

    move_motors_mm(sw_group_nums[2], -80, -80);

    rotate_motors(90, false);

    move_motors_mm(sw_group_nums[2], -560, -560);

     // back up and rotate to throw the bar 
    move_motors_mm(sw_group_nums[2], 30, 30);

    profile_vel_sw[0] = MAX_VEL / 3 - 1;
    profile_vel_sw[1] = MAX_VEL / 3 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_and_release_bar(30);

    profile_vel_sw[0] = MAX_VEL / 4 - 1;
    profile_vel_sw[1] = MAX_VEL / 4 - 1;
    sync_write_velocity(sw_group_nums[1], profile_vel_sw);

    rotate_motors(-30, false);

    move_motors_mm(sw_group_nums[2], -60, -60);
    
    rotate_motors(-90, false);

    move_motors_mm(sw_group_nums[2], 200, 200);

    move_motors_mm(sw_group_nums[2], -300, -300);



    //////////////////////////


}

 // 67s
