
#include "Sensor.h"
#include "sensor_setup.h"

//Global Variables
Sensor_Data accel_data;
Sensor_Data gyro_data;
Sensor_Data mag_data;

//local variables
//accelerometer variables
I2C_Node accel_config_node;
uint8 accel_config_buffer[1];
I2C_Node accel_read_node;
uint8 accel_read_buffer[6];

//functions
void initialize_accel(void *callback)
{
    uint8 address = 0x53;
     
    accel_config_node.callback = 0;
    accel_config_node.data_buffer = accel_config_buffer;
    accel_config_node.data_size = 1;
    accel_config_node.device_address = address;
    accel_config_node.sub_address = 0x2D;
    accel_config_node.mode = WRITE;
    
    accel_config_buffer[0] = (1 << 3); //turn on the accelerometer

    accel_read_node.callback = callback;
    accel_read_node.data_buffer = accel_read_buffer;
    accel_read_node.data_size = 6;
    accel_read_node.device_address = address;
    accel_read_node.sub_address = 0x32;
    accel_read_node.mode = READ;
    accel_read_node.device_id = SID_ACCELEROMETER_0;

    accel_data.channel = I2C_CH_1;
    accel_data.config_nodes = &accel_config_node;
    accel_data.config_nodes_size = 1;
    accel_data.read_nodes = &accel_read_node;
    accel_data.read_nodes_size = 1;
}

void config_accel(void)
{
    initialize_sensor(accel_data);
}

void read_accel(void)
{
    read_sensor(accel_data);
}