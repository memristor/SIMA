#ifndef MOTOR_LOGIC_H
#define MOTOR_LOGIC_H
#include "init.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

void setupMotors();
void sync_write_velocity(int group_num, uint32_t *velocity_data);
void sync_write_acceleration(int group_num, uint32_t *acceleration_data);
void sync_write_gposition(int group_num, uint32_t *position_data);
void set_torque_enable(int port_num, uint8_t id, uint8_t enable);
void set_control_table(int port_num, uint8_t id, uint32_t address, uint32_t data, uint8_t length, int eeprom);
void set_operating_mode(int port_num, uint8_t id, OperatingMode mode);
void set_goal_velocity(int port_num, uint8_t id, uint32_t goal_velocity);
void set_profile_velocity(int port_num, uint8_t id, uint32_t profile_velocity);
void set_profile_acceleration(int port_num, uint8_t id, uint32_t profile_acc);
void set_goal_position(int port_num, uint8_t id, uint32_t goal_position);
void move_motors_mm(int gpos_group_sw_num, double mm1, double mm2);
void rotate_motors(double angle_deg, bool is_pump);
void reset_motors(int vel_group_sw_num, int gpos_group_sw_num,int pos_group_sr_num);

uint32_t read_velocity_limit(int port_num, uint8_t id);
uint32_t read_profile_velocity(int port_num, uint8_t id);
uint32_t read_profile_acceleration(int port_num, uint8_t id);
uint32_t read_profile_gposition(int port_num, uint8_t id);
uint32_t read_present_position(int port_num, uint8_t id);

bool read_moving_status(int port_num, uint8_t id, uint8_t *out_status);

extern SemaphoreHandle_t motor_mutex;

#endif