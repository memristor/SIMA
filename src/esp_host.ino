#include <esp_now.h>

#include <WiFi.h>



uint8_t broadcastAddress[] = {0x48, 0xE7, 0x29, 0xAF, 0xA8, 0x4C}; 



typedef struct struct_message {

  bool enable;

} struct_message;



struct_message sendData;



void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("Send Status: ");

  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

}



void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);



  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW initialization failed!");

    return;

  }



  esp_now_register_send_cb(onSent);



  esp_now_peer_info_t peerInfo;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  peerInfo.channel = 0;

  peerInfo.encrypt = false;



  if (esp_now_add_peer(&peerInfo) != ESP_OK) {

    Serial.println("Failed to add peer");

    return;

  }



  Serial.println("Waiting 5 seconds before sending...");

  delay(5000);



  sendData.enable = true;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));



  if (result == ESP_OK) {

    Serial.println("Enable signal sent!");

  } else {

    Serial.println("Failed to send enable signal");

  }

}



void loop() {

}
