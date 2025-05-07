typedef struct struct_message {
  bool enable;
  bool reset;
} struct_message;

struct_message incomingData;

extern bool motors_enabled;
extern bool motors_reset;


void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int data_len) {
  char macStr[18];
  const uint8_t *mac_addr = recvInfo->src_addr;

  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);

  Serial.print("Packet received from: ");
  Serial.println(macStr);

  if (data_len == sizeof(struct_message)) {
    memcpy(&incomingData, data, sizeof(incomingData));

    Serial.print("Command received: ");
    if (incomingData.reset) {
      motors_enabled = false;
      motors_reset = true;
      Serial.println("RESET");
    } else if (incomingData.enable) {
      motors_enabled = true;
      motors_reset = false;
      Serial.println("START");
    }

    Serial.print("  enable = ");
    Serial.println(incomingData.enable);
    Serial.print("  reset  = ");
    Serial.println(incomingData.reset);
  } else {
    Serial.println("Unexpected data length!");
  }

  Serial.println("-----------------------------");
}


bool initializeESPNOW(){

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW initialization failed!");

    return false;
  } else{
    Serial.println("ESP-NOW initialization successfull!");
  }
  esp_now_register_recv_cb(onReceive);
  return true;
} 