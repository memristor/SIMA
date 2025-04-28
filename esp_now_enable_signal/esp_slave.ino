#include <esp_now.h>

#include <WiFi.h>

#include <Dynamixel2Arduino.h>



// Motor setup

#define SERIJSKA_KANAL Serial2

#define TX_PIN 17

#define RX_PIN 16

#define TX_EN_PIN 4

#define ID1 1  

#define ID2 2  

#define BAUDRATE 115200

#define PROTOCOL 2.0



Dynamixel2Arduino dxl(SERIJSKA_KANAL, TX_EN_PIN);

using namespace ControlTableItem;



typedef struct struct_message {

  bool enable;

} struct_message;



struct_message receivedData;

bool motors_enabled = false;



void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len){

  memcpy(&receivedData, incomingData, sizeof(receivedData));

  if (receivedData.enable) {

    motors_enabled = true;

    Serial.println("Enable signal received! Starting motors...");

  }

}



void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);



  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW initialization failed!");

    return;

  }



  esp_now_register_recv_cb(onReceive);



  // Initialize Dynamixel motors

  SERIJSKA_KANAL.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);

  dxl.begin(BAUDRATE);

  dxl.setPortProtocolVersion(PROTOCOL);

  

  dxl.torqueOff(ID1);

  dxl.setOperatingMode(ID1, OP_PWM);

  dxl.torqueOn(ID1);



  dxl.torqueOff(ID2);

  dxl.setOperatingMode(ID2, OP_PWM);

  dxl.torqueOn(ID2);

  

  Serial.println("Waiting for enable signal...");

}



void loop() {

  if (motors_enabled) {

    dxl.writeControlTableItem(GOAL_PWM, ID1, 850);

    dxl.writeControlTableItem(GOAL_PWM, ID2, -400);

    Serial.println("Motors running...");

    motors_enabled = false;  // Prevent re-executing unless another signal is received

  }

}
