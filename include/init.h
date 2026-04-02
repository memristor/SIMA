#ifndef INIT_H
#define INIT_H

#include "esp_log.h"
#include "driver/gpio.h"
#include "dynamixel_sdk.h"

#define PORT_NAME "UART2"
#define XL430_W250_BAUD 115200 // Wizard postavka motora    
#define PKT_TIMEOUT_uS 1000 // Bilo 200 000, ali kaze Wizard da je return delay motora 500 pa je ovo i vise nego dovoljno
#define DXL_PROTOCOL 2.0
#define DXL_ID_CNT 2
#define MOTOR_1_ID 1
#define MOTOR_2_ID 2
#define MOTOR_3_ID 3
#define TX_EN_PIN GPIO_NUM_18

#define OPMODE_ADDR 11
#define VELOCITY_LIM_ADDR 44
#define TORQUE_ADDR 64
#define GOAL_VELOCITY_ADDR 104
#define PROFILE_ACC_ADDR 108
#define PROFILE_VEL_ADDR 112
#define GOAL_POS_ADDR 116
#define PRESENT_POS_ADDR 132

#define MAX_VEL 256

#define TAG_INIT "UART_INIT"

typedef enum OperatingMode{
  OP_CURRENT = 0,
  OP_VELOCITY = 1,
  OP_POSITION = 3,
  OP_EXTENDED_POSITION = 4,
  OP_CURRENT_BASED_POSITION = 5,
  OP_PWM = 16,
} OperatingMode;

static const uint8_t DXL_ID_LIST[DXL_ID_CNT + 1] = {1, 2, 3};
static const uint8_t dxl_port_num = 2;

extern uint32_t profile_acc_sw[DXL_ID_CNT];
extern uint32_t profile_vel_sw[DXL_ID_CNT];
extern uint32_t goal_pos_sw[DXL_ID_CNT];

extern int sw_group_nums[3];
extern uint32_t present_pos_read[2];
extern int group_num_sr;

int uart_init(const char *port_name, int baud_rate, double timeout_ms);
int setup_sync_read(uint16_t start_address, uint16_t data_length);
int setup_sync_write(uint16_t start_address, uint16_t data_length, uint32_t* data, uint16_t input_length);
void setup_sw(int *sw_group_nums);
void setup_sync();

bool read_position(int group_num, uint32_t pos[2]);

#endif