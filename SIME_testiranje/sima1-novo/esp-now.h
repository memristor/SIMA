#ifndef ESP_NOW_COMMON_H
#define ESP_NOW_COMMON_H

#include <esp_now.h>
#include <WiFi.h>

extern bool motors_enabled;
extern bool motors_reset;

typedef struct struct_message {
  bool enable;
  bool reset;
} struct_message;

extern struct_message incomingData;

void setupESPNOW();
void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int data_len);

#endif
