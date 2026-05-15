#include "servo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool pos_direction = true;

void setup_servo()
{
    ledc_timer_config_t servo_timer = 
    {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_15_BIT,
        .freq_hz = SERVO_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&servo_timer);

    ledc_channel_config_t servo_config =
    {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = STARTING_SERVO_DUTY,
        .hpoint = 0
    };

    ledc_channel_config(&servo_config);
}

void move_motor()
{
    int duty = SERVO_DUTY;

    for (int i = 0; i < TOTAL_CYCLES; i++)
    {
        if (pos_direction == true)
            duty += CYCLE_STEP;
        else
            duty -= CYCLE_STEP;
        
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);

        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(ITERATION_TIME / portTICK_PERIOD_MS);
    }
    
    pos_direction = !pos_direction;
}