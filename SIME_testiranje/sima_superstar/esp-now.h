#ifndef ESP_NOW_COMMON_H
#define ESP_NOW_COMMON_H

#include <esp_now.h>
#include <WiFi.h>

extern bool motors_enabled;
extern bool motors_reset;

extern uint8_t peer1Address[];
extern uint8_t peer2Address[];
extern uint8_t peer3Address[];

typedef struct struct_message {
  bool enable;
  bool reset;
} struct_message;

extern struct_message sendData;
extern struct_message incomingData;

void setupESPNOW();
void sendToAll();
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int data_len);

#endif
