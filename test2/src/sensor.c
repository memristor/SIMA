#include "sensor.h"
#include "cinc_logic.h"
#include "motor_logic.h"

TaskHandle_t check_sensors_handle;

uint32_t remaining[2] = {0, 0};
uint16_t sensors_active = 0;

bool state = false;

void check_sensors_task(void *pvParams)
{
    const TickType_t xFreq = pdMS_TO_TICKS(10);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        xTaskDelayUntil(&xLastWakeTime, xFreq);

        if (end_flag == true)
            vTaskDelete(NULL);
        else if (read_sensors() == true && state == false)
        {
            //read_position(group_num_sr, present_pos_read);
            if (++sensors_active > 20)
            {
                sync_write_gposition(sw_group_nums[2], present_pos_read);

                profile_vel_sw[0] = MIN_VEL_ACC;
                profile_vel_sw[1] = MIN_VEL_ACC;

                sync_write_velocity(sw_group_nums[1], profile_vel_sw);

                remaining[0] = goal_pos_sw[0] - present_pos_read[0];
                remaining[1] = goal_pos_sw[1] - present_pos_read[1];

                state = true;
            }
        }
        else if (read_sensors() == false && state == true)
        {
            profile_vel_sw[0] = MAX_VEL_ACC;
            profile_vel_sw[1] = MAX_VEL_ACC;

            sync_write_velocity(sw_group_nums[1], profile_vel_sw);

            goal_pos_sw[0] = remaining[0] + present_pos_read[0];
            goal_pos_sw[1] = remaining[1] + present_pos_read[1];

            sync_write_gposition(sw_group_nums[2], goal_pos_sw);

            state = false;

            sensors_active = 0;
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