#include "thruster.h"

void CMD_READ_SENSOR_i2c_callback(I2C_Node node)
{
    uint8 data[5];
    
    data[0] = RESP_READ_SENSOR;
    data[1] = node.device_address;
    data[2] = node.sub_address;
    
    get_data_I2C(&node, &(data[3]));
    
    send_packet(PACKET_UART_CH_1, data, sizeof(data));
}

void CMD_READ_ALL_SENSORS_i2c_callback(I2C_Node node)
{
    uint8 data[10];
    
    data[0] = RESP_READ_ALL_SENSORS;
    data[1] = node.device_address;
    
    get_data_I2C(&node, &(data[2]));
    
    send_packet(PACKET_UART_CH_1, data, sizeof(data));
}

void CMD_CHECK_ALIVE_i2c_callback(I2C_Node node)
{
    uint8 data[3];
    uint8 alive_val;
    
    data[0] = RESP_CHECK_ALIVE;
    data[1] = node.device_address;
    
    get_data_I2C(&node, &alive_val);
    
    data[2] = (alive_val == ALIVE_VALUE); 
    
    send_packet(PACKET_UART_CH_1, data, sizeof(data));
}

void thruster_packetizer_callback(uint8* data, uint8 data_size)
{
    THRUSTER_CMD_ID cmd;
    I2C_Node node;
    
    cmd = data[0];
    node.device_address = data[1];
    node.device_id = cmd;
    node.channel = I2C_CH_1;
    
    switch(cmd)
    {
        case CMD_THROTTLE:
            node.callback = NULL;
            node.mode = WRITE;
            node.sub_address = 0x00;
            node.data_size = 2;
            node.data_buffer = &data[2];
            send_I2C(I2C_CH_1, node);
            break;
            
        case CMD_READ_SENSOR:
            node.callback = &CMD_READ_SENSOR_i2c_callback;
            node.mode = READ;
            node.sub_address = data[2];
            node.data_size = 2;
            send_I2C(I2C_CH_1, node);
            break;
            
        case CMD_READ_ALL_SENSORS:
            node.callback = &CMD_READ_ALL_SENSORS_i2c_callback;
            node.mode = READ;
            node.sub_address = SENSOR_REG_START_ADDR;
            node.data_size = NUM_SENSOR_REG;
            send_I2C(I2C_CH_1, node);
            break;
            
        case CMD_CHECK_ALIVE:
            node.callback = &CMD_CHECK_ALIVE_i2c_callback;
            node.mode = READ;
            node.sub_address = ALIVE_REG_ADDR;
            node.data_size = 1;
            send_I2C(I2C_CH_1, node);
            break;
                     
        default: //unrecognized command type
            break;
    }
}
