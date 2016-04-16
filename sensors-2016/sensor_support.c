#include "Sensors.h"
#include "sublibinal.h"

extern I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep;
uint8 m[6], g[8], a[6], channel_data[2];

void configureReadNodes()
{
    gyro_read.callback = &sensorRead;
    gyro_read.data_buffer = g;
    gyro_read.data_size = 8;
    gyro_read.device_address = GYRO_ADDR;
    gyro_read.device_id = SID_GYROSCOPE_1; //Modified each time the node is enqueued
    gyro_read.mode = READ;
    gyro_read.sub_address = 0x1B;
    
    accel_read.callback = &sensorRead;
    accel_read.data_buffer = a;
    accel_read.data_size = 6;
    accel_read.device_address = ACCEL_ADDR;
    accel_read.device_id = SID_ACCELEROMETER_1; //Modified each time the node is enqueued
    accel_read.mode = READ;
    accel_read.sub_address = 0x32;
    
    mag_read.callback = &sensorRead;
    mag_read.data_buffer = m;
    mag_read.data_size = 6;
    mag_read.device_address = MAG_ADDR;
    mag_read.device_id = SID_MAGNOMETER_1; //Modified each time the node is enqueued
    mag_read.mode = READ;
    mag_read.sub_address = 0x03;
    
    depth_prep.callback = NULL; //Cant be called
    depth_prep.data_buffer = NULL;
    depth_prep.data_size = 0;
    depth_prep.device_address = DEPTH_ADDR;
    depth_prep.device_id = SID_DEPTH_1; //Modified each time the node is enqueued
    depth_prep.mode = WRITE;
    depth_prep.sub_address = 0x40; //Command byte to start conversion of depth. Sub-address is not used. OSR = 256
    
    depth_read.callback = &sensorRead;
    depth_read.data_buffer = NULL;
    depth_read.data_size = 0; //24-bit result
    depth_read.device_address = DEPTH_ADDR;
    depth_read.device_id = SID_DEPTH_1; //Modified each time the node is enqueued
    depth_read.mode = READ;
    depth_read.sub_address = 0; //send 0x00 to initiate a depth_read

}

void switchChannel(int channel)
{
    uint8 config_channel = 0;
    
    if (channel == 0)
        config_channel = 0b1;
    else if (channel == 1)
        config_channel = 0b10;
    
    channel_data[0] = config_channel;
    
    I2C_Node i = {0};
    i.callback = NULL;
    i.data_buffer = channel_data;
    i.device_address = MUX_ADDR;
    i.data_size = 1;
    i.device_id = SID_MUX_1;
    i.sub_address = config_channel; //Sub-address does not exist for the mux chip
    i.mode = WRITE;
    
    send_I2C(I2C_CH_1, i);
    
    i.device_id = SID_MUX_2;
    send_I2C(I2C_CH_2, i);
}