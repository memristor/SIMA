#include "esp-now.h"

struct_message incomingData;

void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int data_len) {
    if (data_len == sizeof(struct_message)) {
        memcpy(&incomingData, data, sizeof(incomingData));
        motors_enabled = incomingData.enable;
        motors_reset = incomingData.reset;

        Serial.printf("Received: enable = %d, reset = %d\n", motors_enabled, motors_reset);
    }
}



void setupESPNOW() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_recv_cb(onReceive);
  Serial.println("Izvuci cinc za pocetak kretnje!");
}
