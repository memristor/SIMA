#ifndef TIMER_H
#define TIMER_H

#include "esp_timer.h"
#include "esp_log.h"

#define TIMER_TAG "TIMER.H"

#define DISABLED_TIME_uS 0//89000000
#define RUN_TIME_uS 99000000//10500000

volatile bool end_flag = false;
bool start_flag = false;
bool timer_on = false;

esp_timer_handle_t end_timer;

void IRAM_ATTR timer_call(void *arg)
{
    if (start_flag == false)
    {
        printf("Endflag timer started\n");
        esp_timer_start_once(end_timer, RUN_TIME_uS);
        start_flag = true;
    }
    else
    {
        printf("Endflag = true\n");
        end_flag = true; 
    }                               
}

void init_timer()
{
    const esp_timer_create_args_t se_timer_args =
    {
        .callback = &timer_call,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "end_timer"
    };

    if (esp_timer_create(&se_timer_args, &end_timer) != ESP_OK)
    {
        ESP_LOGI(TIMER_TAG, "Timer init fail.\n");
    } 
    else
        ESP_LOGI(TIMER_TAG, "Timer init success.\n");
}

void start_timer()
{
    if (esp_timer_start_once(end_timer, DISABLED_TIME_uS) != ESP_OK)
    {
        ESP_LOGI(TIMER_TAG, "Timer start fail.\n");
    } 
    else
    {
        ESP_LOGI(TIMER_TAG, "Timer start success.\n");
        timer_on = true;
    }
}

void delay(uint32_t delay_ms)
{
    uint32_t t = esp_timer_get_time();

    while (esp_timer_get_time() - t < delay_ms);
}

#endif