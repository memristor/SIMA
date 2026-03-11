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
#define TX_EN_PIN GPIO_NUM_18

#define OPMODE_ADDR 11
#define VELOCITY_LIM_ADDR 44
#define TORQUE_ADDR 64
#define GOAL_VELOCITY_ADDR 104
#define PROFILE_ACC_ADDR 108
#define PROFILE_VEL_ADDR 112
#define GOAL_POS_ADDR 116
#define PRESENT_POS_ADDR 132

#define TAG_INIT "UART_INIT"

// https://github.com/ROBOTIS-GIT/Dynamixel2Arduino/issues/73
typedef enum OperatingMode{
  OP_CURRENT = 0,
  OP_VELOCITY = 1,
  OP_POSITION = 3,
  OP_EXTENDED_POSITION = 4,
  OP_CURRENT_BASED_POSITION = 5,
  OP_PWM = 16,
} OperatingMode;

const uint8_t DXL_ID_LIST[DXL_ID_CNT] = {1, 2};
const uint8_t dxl_port_num = 2;

uint32_t profile_acc_sw[DXL_ID_CNT] = {0, 0};
uint32_t profile_vel_sw[DXL_ID_CNT] = {0, 0};
uint32_t goal_pos_sw[DXL_ID_CNT] = {0, 0};

//int sw_group_nums[3] = {0, 0, 0}; // PROFILE_ACC, PROFILE_VEL, GOAL_POS

// Function for initialization and opening of desired UART port on ESP
int uart_init(const char *port_name, int baud_rate, double timeout_ms)
{
    int port_num = -1;

    port_num = portHandler(port_name);
    if (port_num < 0)
    {
        ESP_LOGI(TAG_INIT, "Error while using portHandlerEspIdf! Input port name: %s", port_name);
        return -1;
    }
    else
        ESP_LOGI(TAG_INIT, "Successful use of portHandler! Input port name: %s", port_name);
    

    if (!setBaudRate(port_num, baud_rate))
    {
        ESP_LOGI(TAG_INIT, "Error while changing baud rate(port open)!");
        return -1;
    }
    else
        ESP_LOGI(TAG_INIT, "Baudrate set successful, baud: %d!", baud_rate);       

    setPacketTimeoutMSec(port_num, timeout_ms);

    if (!openPort(port_num))
    {
        ESP_LOGI(TAG_INIT, "Error while opening port(maybe opened or installed alrdy)!");
        return -1;
    }
    else
        ESP_LOGI(TAG_INIT, "Port opened successfuly!");

    //Initialize packet handler which is used for sending and recieving packets
    packetHandler();

    if (gpio_get_level(TX_EN_PIN) != 0)
    {
        ESP_LOGI(TAG_INIT, "Error while configuring TX_EN pin!");
        return -1;
    }
    else
        ESP_LOGI(TAG_INIT, "TX_EN pin successfuly configured");

    return port_num;
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

int setup_sync_read(uint16_t start_address, uint16_t data_length)
{
    int group_num = groupSyncRead(dxl_port_num, DXL_PROTOCOL, start_address, data_length);

    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        if (!groupSyncReadAddParam(group_num, DXL_ID_LIST[i]))
        {
            printf("Error adding read parameter for motor id: %d\n", DXL_ID_LIST[i]);
            return -1;
        }
    }

    return group_num;
}

bool read_position(int group_num, uint32_t pos[2])
{
    groupSyncReadTxRxPacket(group_num);

    int recv_cnt;
    for (recv_cnt = 0; recv_cnt < DXL_ID_CNT; recv_cnt++)
    {
        if (!groupSyncReadIsAvailable(group_num, DXL_ID_LIST[recv_cnt], PRESENT_POS_ADDR, 4))
            break;
    }

    if (recv_cnt == DXL_ID_CNT)
    {
        for (uint8_t i = 0; i < DXL_ID_CNT; i++)
        {
            pos[i] = groupSyncReadGetData(group_num, DXL_ID_LIST[i], PRESENT_POS_ADDR, 4);    
        }
        return True;
    }
    else
        return False;
}


int setup_sync_write(uint16_t start_address, uint16_t data_length, uint32_t* data, uint16_t input_length)
{
    int group_num = groupSyncWrite(dxl_port_num, DXL_PROTOCOL, start_address, data_length);

    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        if (!groupSyncWriteAddParam(group_num, DXL_ID_LIST[i], data[i], input_length))
        {
            printf("Error adding write parameter for motor id: %d\n", DXL_ID_LIST[i]);
            return -1; 
        }
    }

    return group_num;
}

void setup_sw(int *sw_group_nums)
{

    sw_group_nums[0] = setup_sync_write(PROFILE_ACC_ADDR, 4, profile_acc_sw, 4);
    if (sw_group_nums[0] == -1)
    {
        printf("Error creating profile acc sw group(in setup_sw)\n");
        return;
    } 
    else
    {
        printf("Profile acc sw group made, group_num: %d\n", sw_group_nums[0]);
    }

    sw_group_nums[1] = setup_sync_write(PROFILE_VEL_ADDR, 4, profile_vel_sw, 4);
    if (sw_group_nums[1] == -1)
    {
        printf("Error creating profile vel sw group(in setup_sw)\n");
        return;
    } 
    else
    {
        printf("Profile vel sw group made, group_num: %d\n", sw_group_nums[1]);
    }

    sw_group_nums[2] = setup_sync_write(GOAL_POS_ADDR, 4, goal_pos_sw, 4);
    if (sw_group_nums[2] == -1)
    {
        printf("Error creating goal pos sw group(in setup_sw)\n");
        return;
    } 
    else
    {
        printf("Goal pos sw group made, group_num: %d\n", sw_group_nums[2]);
    }
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
        printf("Motor 1 velocity: %ld\n", velocity_data[0]);
        printf("Motor 2 velocity: %ld\n", velocity_data[1]);
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
        printf("Motor 1 acceleration: %ld\n", acceleration_data[0]);
        printf("Motor 2 acceleration: %ld\n", acceleration_data[1]);
    }
    else
    {
        printf("Profile acceleration set fail.\n");
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
        printf("Motor 1 position: %ld\n", position_data[0]);
        printf("Motor 2 position: %ld\n", position_data[1]);
    }
    else
    {
        printf("Goal position set fail.\n");
    }
}

void setupMotors() 
{
    int dxl_port_num;

    dxl_port_num = uart_init(PORT_NAME, XL430_W250_BAUD, PKT_TIMEOUT_uS);

    uint8_t motor_count = 0;

    for (uint8_t i = 0; i < DXL_ID_CNT; i++)
    {
        ping(dxl_port_num, DXL_PROTOCOL, DXL_ID_LIST[i]);
        if (packetData[dxl_port_num].communication_result == COMM_SUCCESS)
            motor_count++;
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

    // Set goal position to 0 for motor 1
    //set_control_table(dxl_port_num, MOTOR_1_ID, GOAL_POS_ADDR, 0, 4, 0);
    //vTaskDelay(20 / portTICK_PERIOD_MS);

    // Set goal position to 0 for motor 2
    //set_control_table(dxl_port_num, MOTOR_2_ID, GOAL_POS_ADDR, 0, 4, 0);
    //vTaskDelay(20 / portTICK_PERIOD_MS);

    //printf("Motors set to 0 goal position.\n");
}
