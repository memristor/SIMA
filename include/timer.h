#ifndef TIMER_H
#define TIMER_H

#include "esp_timer.h"
#include "esp_log.h"

#define TIMER_TAG "TIMER.H"

#define RUN_TIME_uS 4000000

volatile bool end_flag = false;

esp_timer_handle_t end_timer;

void IRAM_ATTR timer_call(void *arg)
{
    printf("Endflag = true\n");
    end_flag = true;                                
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
    if (esp_timer_start_once(end_timer, RUN_TIME_uS) != ESP_OK)
    {
        ESP_LOGI(TIMER_TAG, "Timer start fail.\n");
    } 
    else
        ESP_LOGI(TIMER_TAG, "Timer start success.\n");
}

#endif