#include "thruster.h"

THRUSTER_I2C_ADDRESS thruster_list[NUM_THRUSTERS];


void init_thrusters(void)
{
    thruster_list[0] = THRUSTER_ADDR_1;
    thruster_list[1] = THRUSTER_ADDR_2;
    thruster_list[2] = THRUSTER_ADDR_3;
    thruster_list[3] = THRUSTER_ADDR_4;
    thruster_list[4] = THRUSTER_ADDR_5;
    thruster_list[5] = THRUSTER_ADDR_6;
    thruster_list[6] = THRUSTER_ADDR_7;
    thruster_list[7] = THRUSTER_ADDR_8;
}

Error read_thrusters_timer_callback(void)
{
    int i;
    I2C_Node node;
    uint8 data[2];
    
    node.channel = I2C_CH_1;
    node.callback = &read_thrusters_i2c_callback;
    node.mode = READ;
    node.sub_address = 0x02;
    node.data_size = 9;
    
    for (i=0; i < NUM_THRUSTERS; ++i)
    {
        node.device_address = thruster_list[i];
        node.device_id = i;
        
        send_I2C(I2C_CH_1, node);
    }
}

void read_thrusters_i2c_callback(I2C_Node node)
{
    uint8 data[9];
    
    get_data_I2C(&node, data);
    
    send_packet(PACKET_UART_CH_1, data, 9);
}

void thruster_packetizer_callback(uint8* data, uint8 data_size)
{
    uint8 packet_type;
    I2C_Node node;
    uint8 which_thruster;
    
    packet_type = data[0];
    
    switch(packet_type)
    {
        case THRUSTER_P_COMMAND:
            LATBSET = 1 << 5;
            which_thruster = data[1];
            node.channel = I2C_CH_1;
            node.callback = NULL;
            node.mode = WRITE;
            node.sub_address = 0x00;
            node.data_size = 2;
            node.device_address = thruster_list[which_thruster];
            node.device_id = which_thruster;
            node.data_buffer = &data[2];
            send_I2C(I2C_CH_1, node);
            break;
            
        default: //unrecognized packet type
            break;
    }
}
