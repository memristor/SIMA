#include "sensor.h"
#include "cinc_logic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t check_sensors_handle;

void check_sensors_task(void *pvParams)
{
    const TickType_t xFreq = pdMS_TO_TICKS(10);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        xTaskDelayUntil(&xLastWakeTime, xFreq);

        if (end_flag == true)
            vTaskDelete(NULL);
        else if (read_sensors() == true)
        {
            printf("Sensors high!\n");          // Nemam senzore
        }

    }
    
}

void sensor_init()
{
    gpio_reset_pin(SENS1);
    gpio_reset_pin(SENS2);

    gpio_set_direction(SENS1, GPIO_MODE_INPUT);
    gpio_set_direction(SENS2, GPIO_MODE_INPUT);
}

void create_check_sensors_task()
{
    BaseType_t creation_result;

    creation_result = xTaskCreatePinnedToCore(
                                            check_sensors_task,
                                            "CHECK_SENSORS_TASK",
                                            4096,
                                            NULL,
                                            2,                 
                                            &check_sensors_handle,
                                            0                       // Stavljeno na CORE0 sa tajmerom
                                            );                     

    if (creation_result != pdPASS)
    {
        printf("Check sensors task creation error.\n");
    }
    else
    {
        printf("Check sensors task created.\n");
    }
}

bool read_sensors()
{
    return gpio_get_level(SENS1) || gpio_get_level(SENS2);
}