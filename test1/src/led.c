#include "led.h"
#include "sensor.h"
#include "cinc_logic.h"
#include "motor_logic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t check_led_handle;

void check_led_task(void *pvParams)
{
    const TickType_t xFreq = pdMS_TO_TICKS(10);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        xTaskDelayUntil(&xLastWakeTime, xFreq);

        if (end_flag == true)
            vTaskDelete(NULL);
        else
            check_led();
    }
}

void create_check_led_task()
{
    BaseType_t creation_result;

    creation_result = xTaskCreatePinnedToCore(
                                            check_led_task,
                                            "CHECK_LED_TASK",
                                            4096,
                                            NULL,
                                            1,                 
                                            &check_led_handle,
                                            0
                                            );                     

    if (creation_result != pdPASS)
    {
        printf("Check LED task creation error.\n");
    }
    else
    {
        printf("Check LED task created.\n");
    }
}