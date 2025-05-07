#ifndef INIT_H
#define INIT_H

#include <Dynamixel2Arduino.h>

#define TX_PIN        17
#define RX_PIN        16
#define DXL_DIR_PIN   18
#define DXL_SERIAL    Serial2

#define ID1 1
#define ID2 2
#define BAUD_RATE 115200
#define PROTOCOL 2.0

extern int32_t pos[2];

extern Dynamixel2Arduino dxl;
using namespace ControlTableItem;

const uint8_t DXL_ID_CNT = 2;
const uint8_t DXL_ID_LIST[DXL_ID_CNT] = {ID1, ID2};

struct sr_data_t {
  int32_t present_position;
} __attribute__((packed));

struct sw_data_t_vel {
  int32_t goal_velocity;
} __attribute__((packed));

struct sw_data_t_acc {
  int32_t goal_acceleration;
} __attribute__((packed));

struct sw_data_t_pos {
  int32_t goal_position;
} __attribute__((packed));

extern sr_data_t sr_data[DXL_ID_CNT];
extern sw_data_t_vel sw_data_vel[DXL_ID_CNT];
extern sw_data_t_acc sw_data_acc[DXL_ID_CNT];
extern sw_data_t_pos sw_data_pos[DXL_ID_CNT];

extern DYNAMIXEL::InfoSyncReadInst_t sr_infos;
extern DYNAMIXEL::XELInfoSyncRead_t info_xels_sr[DXL_ID_CNT];

extern DYNAMIXEL::InfoSyncWriteInst_t sw_infos_vel;
extern DYNAMIXEL::InfoSyncWriteInst_t sw_infos_acc;
extern DYNAMIXEL::InfoSyncWriteInst_t sw_infos_pos;

extern DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_vel[DXL_ID_CNT];
extern DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_acc[DXL_ID_CNT];
extern DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_pos[DXL_ID_CNT];

void setupSyncRead();
void setupSyncAcceleration();
void setupSyncVelocity();
void setupSyncPosition();
void setupSync();

bool readPosition(int32_t pos[2]);

#endif
