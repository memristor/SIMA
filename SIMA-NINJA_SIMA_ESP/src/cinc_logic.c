#include "cinc_logic.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

bool prev_cinc = false;
bool cinc      = false;
bool strat     = false;

strategy_t strategy = NO_BARS_SAFE; // HOMOLOGACIJA, TEST_ROTATION, NO_BARS_SAFE, NO_BARS_SAFE_PUSH_FIRST, NO_BARS_AGR, BARS_2, BARS_4

void init_strat()
{
    gpio_reset_pin(YELLOW_PIN);
    gpio_reset_pin(BLUE_PIN);
    gpio_reset_pin(CINC_PIN);
    gpio_reset_pin(STRAT_PIN);

    gpio_set_direction(YELLOW_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_PIN, GPIO_MODE_OUTPUT);

    gpio_set_direction(CINC_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(STRAT_PIN, GPIO_MODE_INPUT);

    gpio_input_enable(CINC_PIN);
    gpio_input_enable(STRAT_PIN);


    gpio_set_pull_mode(STRAT_PIN, GPIO_PULLUP_ONLY);
    gpio_pullup_en(STRAT_PIN);
}

void check_led()
{
    cinc = gpio_get_level(CINC_PIN);

    if (!cinc)
    {
        gpio_set_level(BLUE_PIN, false);
        gpio_set_level(YELLOW_PIN, false);
        
        strat = gpio_get_level(STRAT_PIN);

        if (strat)
            flash_blue();
        else
            flash_yellow();
    }
    else
    {
        strat = gpio_get_level(STRAT_PIN);

        if (strat)
        {
            gpio_set_level(BLUE_PIN, true);
            gpio_set_level(YELLOW_PIN, false);
        }
        else
        {
            gpio_set_level(BLUE_PIN, false);
            gpio_set_level(YELLOW_PIN, true);
        }
    }
}

void check_strat()
{
    strat = gpio_get_level(STRAT_PIN);

    if (strat)
    {
        switch(strategy){
            case HOMOLOGACIJA:
                SIMA_N_HOMOLOGACIJA();
                break;
            case TEST_ROTATION:
                SIMA_N_TEST_ROTATION();
                break;
            case NO_BARS_SAFE:
                SIMA_N_BLUE_NO_BARS_SAFE();
                break;
            case NO_BARS_SAFE_PUSH_FIRST:
                SIMA_N_BLUE_NO_BARS_SAFE_PUSH_FIRST();
                break;
            case NO_BARS_AGR:
                SIMA_N_BLUE_NO_BARS_AGRESSIVE();
                break;
            case BARS_2:
                SIMA_N_BLUE2();
                break;
            case BARS_4:
                SIMA_N_BLUE4();
                break;
        }
    }
    else 
    {
        switch(strategy){
            case HOMOLOGACIJA:
                SIMA_N_HOMOLOGACIJA();
                break;
            case TEST_ROTATION:
                SIMA_N_TEST_ROTATION();
                break;
            case NO_BARS_SAFE:
                SIMA_N_YELLOW_NO_BARS_SAFE();
                break;
            case NO_BARS_SAFE_PUSH_FIRST:
                SIMA_N_YELLOW_NO_BARS_SAFE_PUSH_FIRST();
                break;
            case NO_BARS_AGR:
                SIMA_N_YELLOW_NO_BARS_AGRESSIVE();
                break;
            
            case BARS_2:
                SIMA_N_YELLOW2();
                break;
            case BARS_4:
                SIMA_N_YELLOW4();
                break;
        }
    }
}

void flash_blue()
{
    gpio_set_level(BLUE_PIN, true);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(BLUE_PIN, false);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void flash_yellow()
{
    gpio_set_level(YELLOW_PIN, true);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(YELLOW_PIN, false);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}