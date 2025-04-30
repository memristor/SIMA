#include <esp_now.h>
#include <WiFi.h>

// MAC addresses of the receiver ESP32 boards
uint8_t peer1Address[] = {0x08, 0xD1, 0xF9, 0x35, 0xA9, 0x78};
uint8_t peer2Address[] = {0xC4, 0xDD, 0x57, 0xCA, 0xAF, 0xF0};

// Updated struct to include both enable and reset
typedef struct struct_message {
  bool enable;
  bool reset;
} struct_message;

struct_message sendData;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status to ");
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(": ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void addPeer(const uint8_t *mac) {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.print("Failed to add peer: ");
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", mac[i]);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
  }
}

void sendToAll() {
  esp_now_send(peer1Address, (uint8_t *)&sendData, sizeof(sendData));
  esp_now_send(peer2Address, (uint8_t *)&sendData, sizeof(sendData));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_send_cb(onSent);

  // Add both peers
  addPeer(peer1Address);
  addPeer(peer2Address);

  Serial.println("Type 'start' or 'reset' to send a command.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equalsIgnoreCase("start")) {
      sendData.enable = true;
      sendData.reset = false;
      sendToAll();
      Serial.println("Sent: START");
    } else if (input.equalsIgnoreCase("reset")) {
      sendData.enable = false;
      sendData.reset = true;
      sendToAll();
      Serial.println("Sent: RESET");
    } else {
      Serial.println("Unknown command. Use: start or reset");
    }
  }
}
