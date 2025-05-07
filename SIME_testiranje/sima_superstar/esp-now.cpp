#include "esp-now.h"

uint8_t peer1Address[] = { 0xC4, 0xDD, 0x57, 0xCA, 0xAF, 0xF0 };
uint8_t peer2Address[] = { 0x3C, 0x8A, 0x1F, 0x9D, 0x31, 0xC4 };
uint8_t peer3Address[] = { 0x78, 0x42, 0x1C, 0x67, 0x95, 0x10 };
uint8_t peer4Address[] = { 0x78, 0x42, 0x1C, 0x68, 0x3D, 0x08 };

struct_message sendData;
struct_message incomingData;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Message sent to: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" — Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

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

    sendData.enable = incomingData.enable;
    sendData.reset = incomingData.reset;
    sendToAll();

    Serial.print("  enable = ");
    Serial.println(incomingData.enable);
    Serial.print("  reset  = ");
    Serial.println(incomingData.reset);
  } else {
    Serial.println("Unexpected data length!");
  }

  Serial.println("-----------------------------");
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
  esp_now_send(peer3Address, (uint8_t *)&sendData, sizeof(sendData));
  esp_now_send(peer4Address, (uint8_t *)&sendData, sizeof(sendData));
}

void setupESPNOW() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_send_cb(onSent);
  esp_now_register_recv_cb(onReceive);

  addPeer(peer1Address);
  addPeer(peer2Address);
  addPeer(peer3Address);
  addPeer(peer4Address);

  Serial.println("Izvuci cinc za pocetak kretnje!");
}
