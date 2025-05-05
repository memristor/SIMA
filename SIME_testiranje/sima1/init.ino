#define TX_PIN 17
#define RX_PIN 16
#define DXL_DIR_PIN 18
#define DXL_SERIAL Serial2

#define ID1 1
#define ID2 2
#define BAUD_RATE 115200
#define PROTOCOL 2.0


Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;



const uint8_t DXL_ID_CNT = 2;
const uint8_t DXL_ID_LIST[DXL_ID_CNT] = {1, 2};
const uint16_t user_pkt_buf_cap = 128;
uint8_t user_pkt_buf[user_pkt_buf_cap];

int32_t pos[2];

//syncRead
typedef struct sr_data{
  int32_t present_position;
} __attribute__((packed)) sr_data_t;

sr_data_t sr_data[DXL_ID_CNT];

DYNAMIXEL::InfoSyncReadInst_t sr_infos;
DYNAMIXEL::XELInfoSyncRead_t info_xels_sr[DXL_ID_CNT];

//syncWrite
typedef struct sw_data_vel{
  int32_t goal_velocity;
} __attribute__((packed)) sw_data_t_vel;

typedef struct sw_data_acc{
  int32_t goal_acceleration;
} __attribute__((packed)) sw_data_t_acc;

typedef struct sw_data_pos{
  int32_t goal_position;
} __attribute__((packed)) sw_data_t_pos;

sw_data_t_vel sw_data_vel[DXL_ID_CNT];
sw_data_t_acc sw_data_acc[DXL_ID_CNT];
sw_data_t_pos sw_data_pos[DXL_ID_CNT];

DYNAMIXEL::InfoSyncWriteInst_t sw_infos_vel;
DYNAMIXEL::InfoSyncWriteInst_t sw_infos_acc;
DYNAMIXEL::InfoSyncWriteInst_t sw_infos_pos;
DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_vel[DXL_ID_CNT];
DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_acc[DXL_ID_CNT];
DYNAMIXEL::XELInfoSyncWrite_t info_xels_sw_pos[DXL_ID_CNT];

void setupSyncRead(){
  sr_infos.packet.p_buf = user_pkt_buf;
  sr_infos.packet.buf_capacity = user_pkt_buf_cap;
  sr_infos.packet.is_completed = false;
  sr_infos.addr = 132;
  sr_infos.addr_length = 4;
  sr_infos.p_xels = info_xels_sr;
  sr_infos.xel_count = 0;  

  for(int i = 0; i < DXL_ID_CNT; i++){
    info_xels_sr[i].id = DXL_ID_LIST[i];
    info_xels_sr[i].p_recv_buf = (uint8_t*)&sr_data[i];
    sr_infos.xel_count++;
  }
  sr_infos.is_info_changed = true;
}

void setupSyncAcceleration(){
  sw_infos_acc.packet.p_buf = nullptr;
  sw_infos_acc.packet.is_completed = false;
  sw_infos_acc.addr = 108;
  sw_infos_acc.addr_length = 4;
  sw_infos_acc.p_xels = info_xels_sw_acc;
  sw_infos_acc.xel_count = 0;

  for (int i = 0; i < DXL_ID_CNT; i++) {
  info_xels_sw_acc[i].id = DXL_ID_LIST[i];
  info_xels_sw_acc[i].p_data = (uint8_t*)&sw_data_acc[i].goal_acceleration;
  sw_infos_acc.xel_count++;
  }
  sw_infos_acc.is_info_changed = true;
}

void setupSyncVelocity(){
  sw_infos_vel.packet.p_buf = nullptr;
  sw_infos_vel.packet.is_completed = false;
  sw_infos_vel.addr = 112;
  sw_infos_vel.addr_length = 4;
  sw_infos_vel.p_xels = info_xels_sw_vel;
  sw_infos_vel.xel_count = 0;

  for (int i = 0; i < DXL_ID_CNT; i++) {
  info_xels_sw_vel[i].id = DXL_ID_LIST[i];
  info_xels_sw_vel[i].p_data = (uint8_t*)&sw_data_vel[i].goal_velocity;
  sw_infos_vel.xel_count++;
  }
  sw_infos_vel.is_info_changed = true;
}

void setupSyncPosition(){
  sw_infos_pos.packet.p_buf = nullptr;
  sw_infos_pos.packet.is_completed = false;
  sw_infos_pos.addr = 116;
  sw_infos_pos.addr_length = 4;
  sw_infos_pos.p_xels = info_xels_sw_pos;
  sw_infos_pos.xel_count = 0;

  for (int i = 0; i < DXL_ID_CNT; i++) {
    info_xels_sw_pos[i].id = DXL_ID_LIST[i];
    info_xels_sw_pos[i].p_data = (uint8_t*)&sw_data_pos[i].goal_position;
    sw_infos_pos.xel_count++;
  }
  sw_infos_pos.is_info_changed = true;
}

void setupSync(){
  setupSyncRead();
  setupSyncVelocity();
  setupSyncPosition();
  setupSyncAcceleration();
}

bool readPosition(int32_t pos[2]) {
  int recv_cnt = dxl.syncRead(&sr_infos);

  if (recv_cnt == DXL_ID_CNT) {
    for (int i = 0; i < DXL_ID_CNT; i++) {
      pos[i] = sr_data[i].present_position;
    }
    return true;
  } else {
    return false;
  }
}
