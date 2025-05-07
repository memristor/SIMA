#include "esp_timer.h"

volatile bool endFlag = false;
volatile bool flashLedsFlag = false;


// Timer callback
void IRAM_ATTR onTimer(void* arg) {
  endFlag = true;
}

esp_timer_handle_t timer;

void initTimer(){

  // Timer configuration
  const esp_timer_create_args_t timer_args = {
    .callback = &onTimer,
    .arg = nullptr,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "shutdown_timer"
  };

  esp_timer_create(&timer_args, &timer);
}

void callTimer(){
   esp_timer_start_once(timer, 14500000); // 14.5 seconds in microseconds
}
