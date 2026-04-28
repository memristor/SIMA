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
}

void SIMA_N_YELLOW()
{
    
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
    vTaskDelay(500/portTICK_PERIOD_MS);
    rotate_motors(45, false);
    move_motors_mm(sw_group_nums[2], -260, -260);
    move_motors_mm(sw_group_nums[2], 80, 80);
    rotate_motors(79, false);
    move_motors_mm(sw_group_nums[2], 159, 159);
    rotate_motors(-88, false);
    move_motors_mm(sw_group_nums[2], -420, -420);
    rotate_motors(86, false);
    vTaskDelay(5000/portTICK_PERIOD_MS);
    
    move_motors_mm(sw_group_nums[2], -251, -251);
    move_motors_mm(sw_group_nums[2], 210, 210);
    rotate_motors(89, false);
    move_motors_mm(sw_group_nums[2], -430, -430);
    rotate_motors(-90, false);
    move_motors_mm(sw_group_nums[2], -250, -250);

    
    /*
    //brkicev predlog
    // |
    // |
    // V
    move_motors_mm(sw_group_nums[2],-175, -175);
    move_motors_mm(sw_group_nums[2],60, 60);
    rotate_motors(40,false);
    move_motors_mm(sw_group_nums[2],-90, -90);
    move_motors_mm(sw_group_nums[2],40, 40);    
    rotate_motors(40,false);
    move_motors_mm(sw_group_nums[2],-450, -450);
    */

    //ovo je kod za kupljenje pumpom
    // |
    // |
    // V
    /*
    rotate_motors(-15, false);
    move_motors_mm(sw_group_nums[2],-160, -160);
    rotate_motors(15, false);
    move_motors_mm(sw_group_nums[2],-262, -262);
    pick_up_bar();
    rotate_pump(90);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_pump(-90);
    //razlika u pumpi od pocetne pozicije -18
    vTaskDelay(1000/portTICK_PERIOD_MS);
    move_motors_mm(sw_group_nums[2],50, 50);
    rotate_motors(15, true);
    move_motors_mm(sw_group_nums[2],65, 65);
    rotate_motors(-15, true);
    move_motors_mm(sw_group_nums[2],285, 285);
    rotate_motors(90, true);
    vTaskDelay(500/portTICK_PERIOD_MS);
    release_bar();
    move_motors_mm(sw_group_nums[2], 20, 20);
    vTaskDelay(500/portTICK_PERIOD_MS);
    rotate_motors(-90, false);
    vTaskDelay(500/portTICK_PERIOD_MS);
    //zavrsio ostavljanje prve daske
    move_motors_mm(sw_group_nums[2],-470, -470);
    pick_up_bar();
    rotate_pump(90);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_pump(-90);
    //razlika u pumpi od pocetne pozicije 0
    vTaskDelay(1000/portTICK_PERIOD_MS);
    rotate_motors(-15, true);
    move_motors_mm(sw_group_nums[2],50, 50);
    rotate_motors(15, true);
    move_motors_mm(sw_group_nums[2],400, 400);
    rotate_motors(-90, true);
    release_bar();
    rotate_motors(75, false);
    move_motors_mm(sw_group_nums[2],-650, -650);
    rotate_motors(-6.5, false);
    vTaskDelay(500/portTICK_PERIOD_MS);
    move_motors_mm(sw_group_nums[2],610, 610);
    rotate_motors(100, false);
    */

}

void SIMA_N_BLUE()
{   
    //ovo je kod za guranje - STO NA KOM JE VELIKI PROKLIZAVAO (onaj dalje od naseg ormana)
    // |
    // |
    // V

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    move_motors_mm(sw_group_nums[2], -40, -40);

    pick_up_bar();

    move_motors_mm(sw_group_nums[2], -250, -250);

    release_bar();


    
    // move_motors_mm(sw_group_nums[2], 50, 50);
    // rotate_motors(-25, false);
    // move_motors_mm(sw_group_nums[2], 100, 100);
    // rotate_motors(35, false);
    // move_motors_mm(sw_group_nums[2], 200, 200);
    // move_motors_mm(sw_group_nums[2], -30, -30);
    // vTaskDelay(500/portTICK_PERIOD_MS);
    // rotate_motors(-45, false);
    // move_motors_mm(sw_group_nums[2], -260, -260);
    // move_motors_mm(sw_group_nums[2], 80, 80);
    // rotate_motors(-79.5, false);
    // move_motors_mm(sw_group_nums[2], 160, 160);
    // rotate_motors(91, false);
    // move_motors_mm(sw_group_nums[2], -420, -420);
    // rotate_motors(-90, false);
    // vTaskDelay(5000/portTICK_PERIOD_MS);
    
    // move_motors_mm(sw_group_nums[2], -255, -255);
    // move_motors_mm(sw_group_nums[2], 210, 210);
    // rotate_motors(-89, false);
    // move_motors_mm(sw_group_nums[2], -430, -430);
    // rotate_motors(90, false);
    // move_motors_mm(sw_group_nums[2], -260, -260);

    //brkicev predlog
    // |
    // |
    // V
    /*
    move_motors_mm(sw_group_nums[2],-160, -160);
    move_motors_mm(sw_group_nums[2],30, 30);
    rotate_motors(80,false);
    move_motors_mm(sw_group_nums[2],-450, -450);
   */



    //ovo je kod za kupljenje pumpom
    // |
    // |
    // V
    /*
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
    */
}


