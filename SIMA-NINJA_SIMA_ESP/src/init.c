#include "init.h"

uint32_t profile_acc_sw[DXL_ID_CNT];
uint32_t profile_vel_sw[DXL_ID_CNT];
uint32_t goal_pos_sw[DXL_ID_CNT];

int sw_group_nums[3];
uint32_t present_pos_read[2];
int group_num_sr;

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

void setup_sync()
{
    uart_init(PORT_NAME, XL430_W250_BAUD, PKT_TIMEOUT_uS);

    group_num_sr = setup_sync_read(PRESENT_POS_ADDR, 4);

    setup_sw(sw_group_nums);
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