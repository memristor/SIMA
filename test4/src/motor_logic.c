#include "init.h"
#include "motor_logic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "math.h"

#define WHEEL_DIAMETER_1_mm 88
#define WHEEL_DIAMETER_2_mm 88
#define WHEELS_DISTANCE_mm 95

const double TICKS_PER_MM_1 = 4095.0 / (M_PI * WHEEL_DIAMETER_1_mm);
const double TICKS_PER_MM_2 = 4095.0 / (M_PI * WHEEL_DIAMETER_2_mm);

void setupMotors() 
{
    uint8_t motor_count = 0;

    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        ping(dxl_port_num, DXL_PROTOCOL, DXL_ID_LIST[i]);
        if (packetData[dxl_port_num].communication_result == COMM_SUCCESS)
        {
            motor_count++;
            //printf("Motor id: %d", DXL_ID_LIST[i]);
        }
    }
    printf("Motor count: %d\n", motor_count);
    if (motor_count == DXL_ID_CNT)
        printf("Motor connection established!\n");
    else
    {
        printf("Failed to connect to motors.\n");
        while (1);
    }

    // Set operating mode for motor 1
    set_operating_mode(dxl_port_num, MOTOR_1_ID, OP_EXTENDED_POSITION);

    // Set operating mode for motor 2
    set_operating_mode(dxl_port_num, MOTOR_2_ID, OP_EXTENDED_POSITION);
    
    printf("Motors set to Extended Position Control Mode.\n");
}

void sync_write_velocity(int group_num, uint32_t *velocity_data)
{
    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        if (!groupSyncWriteChangeParam(group_num, DXL_ID_LIST[i], velocity_data[i], 4, 0))
        {
            printf("SyncWrite velocity param change fail!\n");
            printf("ID: %d\n", DXL_ID_LIST[i]);
            return;
        }
    }

    groupSyncWriteTxPacket(group_num);

    if (packetData[dxl_port_num].communication_result == COMM_SUCCESS)
    {
        printf("Profile velocity set successful!\n");
        printf("Motor 1 velocity: %ld\n", read_profile_velocity(dxl_port_num, MOTOR_1_ID));
        printf("Motor 2 velocity: %ld\n", read_profile_velocity(dxl_port_num, MOTOR_2_ID));
    }
    else
    {
        printf("Profile velocity set fail.\n");
    }
}

void sync_write_acceleration(int group_num, uint32_t *acceleration_data)
{
    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        if (!groupSyncWriteChangeParam(group_num, DXL_ID_LIST[i], acceleration_data[i], 4, 0))
        {
            printf("SyncWrite acceleration param change fail!\n");
            printf("ID: %d\n", DXL_ID_LIST[i]);
            return;
        }
    }

    groupSyncWriteTxPacket(group_num);

    if (packetData[dxl_port_num].communication_result == COMM_SUCCESS)
    {
        printf("Profile acceleration set successful!\n");
        printf("Motor 1 acceleration: %ld\n", read_profile_acceleration(dxl_port_num, MOTOR_1_ID));
        printf("Motor 2 acceleration: %ld\n", read_profile_acceleration(dxl_port_num, MOTOR_2_ID));
    }
    else
    {
        printf("Profile acceleration set fail.\n");
        printf("Error code: %d", packetData[dxl_port_num].communication_result);
    }
}

void sync_write_gposition(int group_num, uint32_t *position_data)
{
    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        if (!groupSyncWriteChangeParam(group_num, DXL_ID_LIST[i], position_data[i], 4, 0))
        {
            printf("SyncWrite goal position param change fail!\n");
            printf("ID: %d\n", DXL_ID_LIST[i]);
            return;
        }
    }

    groupSyncWriteTxPacket(group_num);

    if (packetData[dxl_port_num].communication_result == COMM_SUCCESS)
    {
        printf("Goal position set successful!\n");
        printf("Motor 1 position: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_1_ID));
        printf("Motor 2 position: %ld\n", read_profile_gposition(dxl_port_num, MOTOR_2_ID));
    }
    else
    {
        printf("Goal position set fail.\n");
    }
}

void set_torque_enable(int port_num, uint8_t id, uint8_t enable)
{
    write1ByteTxOnly(port_num, DXL_PROTOCOL, id, TORQUE_ADDR, enable);

    int result = getLastTxRxResult(port_num, DXL_PROTOCOL);

    // packetData[dxl_port_num].communication_result

    if (result != COMM_SUCCESS)
    {
        printf("Writing to motor_id: %d from set_torque_enable failed\n", id);
        printf("Error code: %s\n", getTxRxResult(DXL_PROTOCOL, result));
        return;
    } 
    else
    {
        printf("Torque set succesful, value set: %d\n", enable);
        return;
    }
}

void set_control_table(int port_num, uint8_t id, uint32_t address, uint32_t data, uint8_t length, int eeprom) // eeprom version
{
    if (eeprom == 1)    // eeprom 1 je upis u eeprom deo, pa mora Torque_en na 0
        set_torque_enable(port_num, id, 0);
    
    switch (length)
    {
    case 1:
        write1ByteTxOnly(port_num, DXL_PROTOCOL, id, address, data);
        break;
    case 2:
        write2ByteTxOnly(port_num, DXL_PROTOCOL, id, address, data);
        break;
    case 4:
        write4ByteTxOnly(port_num, DXL_PROTOCOL, id, address, data);
        break;
    default:
        printf("Length is not supported // set_control_table\n");
        break;
    }

    set_torque_enable(port_num, id, 1);
}

void set_operating_mode(int port_num, uint8_t id, OperatingMode mode)
{
    set_control_table(port_num, id, OPMODE_ADDR, mode, 1, 1);

    int result = getLastTxRxResult(port_num, DXL_PROTOCOL);

    // packetData[dxl_port_num].communication_result

    if (result != COMM_SUCCESS)
    {
        printf("Writing to motor_id: %d from set_operating_mode failed\n", id);
        printf("Error code: %s\n", getTxRxResult(DXL_PROTOCOL, result));
        return;
    } 
    else
    {
        printf("Mode set succesful, value set: ");
        switch (mode)
        {
        case 0:
            printf("OP_CURRENT\n");
            break;
        case 1:
            printf("OP_VELOCITY\n");
            break;
        case 3:
            printf("OP_POSITION\n");
            break;
        case 4:
            printf("OP_EXTENDED_POSITION\n");
            break;
        case 16:
            printf("OP_PWM\n");
            break;
        default:
            printf("Mode not valid\n");
            break;
        }
        return;
    }
}

void set_goal_velocity(int port_num, uint8_t id, uint32_t goal_velocity)
{
    set_control_table(port_num, id, GOAL_VELOCITY_ADDR, goal_velocity, 4, 0);

    int result = getLastTxRxResult(dxl_port_num, DXL_PROTOCOL);

    if (result != COMM_SUCCESS)
    {
        printf("Writing to motor id: %d from set_goal_velocity failed.\n", id);
        printf("Error code: %s\n", getTxRxResult(DXL_PROTOCOL, result));
        return;
    }
    else
    {
        printf("Goal velocity set successful, value set: %ld\n", goal_velocity);
    }
}

void move_motors_mm(int gpos_group_sw_num, double mm1, double mm2)
{
    uint32_t offset1 = (uint32_t) roundf(mm1 * TICKS_PER_MM_1);
    uint32_t offset2 = (uint32_t) roundf(mm2 * TICKS_PER_MM_2);

    printf("Offset 1: %ld\n", offset1);
    printf("Offset 2: %ld\n", offset2);

    read_position(group_num_sr, present_pos_read);

    goal_pos_sw[0] = present_pos_read[0] - offset1;
    goal_pos_sw[1] = present_pos_read[1] + offset2;

    sync_write_gposition(gpos_group_sw_num, goal_pos_sw);

    /*do
    {
        if (!read_position(group_num_sr, present_pos_read))
            continue;
        vTaskDelay(20 / portTICK_PERIOD_MS);
    } while (abs(goal_pos_sw[0] - present_pos_read[0]) > 20 || abs(goal_pos_sw[1] - present_pos_read[1]) > 20);*/
    while(true)
    {
        if (!read_position(group_num_sr, present_pos_read))
            continue;

        vTaskDelay(20 / portTICK_PERIOD_MS);
        int64_t pos_sw = goal_pos_sw[0];
        int64_t pos_read = present_pos_read[0];
        if (llabs(pos_sw - pos_read) <= 20)
            break;

        pos_sw = goal_pos_sw[1];
        pos_read = present_pos_read[1];
        if (llabs(pos_sw - pos_read) <= 20)
            break;
    }

    // Deo koji proverava kraj kretanja stavljen unutar stop_motors_end task-a u strategy.h
      
}

void rotate_motors(double angle_deg)
{
    double arc_mm = WHEELS_DISTANCE_mm * (angle_deg * M_PI / 360);

    move_motors_mm(sw_group_nums[2], arc_mm, -arc_mm);
}

void reset_motors(int vel_group_sw_num, int gpos_group_sw_num, int pos_group_sr_num)
{
    profile_vel_sw[0] = -MAX_VEL;
    profile_vel_sw[1] =  MAX_VEL;

    sync_write_velocity(vel_group_sw_num, profile_vel_sw);

    goal_pos_sw[0] = 0;
    goal_pos_sw[1] = 0;

    sync_write_gposition(gpos_group_sw_num, goal_pos_sw);

    while (1)
    {
        if (!read_position(pos_group_sr_num, present_pos_read))
            continue;
        if ((goal_pos_sw[0] - present_pos_read[0]) > 20 || (goal_pos_sw[1] - present_pos_read[1]) > 20)
            break;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    printf("Motors reset success!\n");
}

uint32_t read_velocity_limit(int port_num, uint8_t id)
{
    uint32_t rec_data;

    rec_data = read4ByteTxRx(port_num, DXL_PROTOCOL, id, VELOCITY_LIM_ADDR);

    return rec_data;
}

uint32_t read_profile_velocity(int port_num, uint8_t id)
{
    uint32_t rec_data;

    rec_data = read4ByteTxRx(port_num, DXL_PROTOCOL, id, PROFILE_VEL_ADDR);

    return rec_data;
}

uint32_t read_profile_acceleration(int port_num, uint8_t id)
{
    uint32_t rec_data;

    rec_data = read4ByteTxRx(port_num, DXL_PROTOCOL, id, PROFILE_ACC_ADDR);

    return rec_data;
}

uint32_t read_profile_gposition(int port_num, uint8_t id)
{
    uint32_t rec_data;

    rec_data = read4ByteTxRx(port_num, DXL_PROTOCOL, id, GOAL_POS_ADDR);

    return rec_data;
}
