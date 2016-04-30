#include "Sensors.h"
#include "sublibinal.h"

extern I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep, temp_read, temp_prep;

void configureReadNodes()
{
    gyro_read.callback = &sensorRead;
    gyro_read.data_buffer = NULL;
    gyro_read.data_size = 8;
    gyro_read.device_address = GYRO_ADDR;
    gyro_read.device_id = SID_GYROSCOPE_1; //Modified each time the node is enqueued
    gyro_read.mode = READ;
    gyro_read.sub_address = 0x1B;
    
    accel_read.callback = &sensorRead;
    accel_read.data_buffer = NULL;
    accel_read.data_size = 6;
    accel_read.device_address = ACCEL_ADDR;
    accel_read.device_id = SID_ACCELEROMETER_1; //Modified each time the node is enqueued
    accel_read.mode = READ;
    accel_read.sub_address = 0x32;
    
    mag_read.callback = &sensorRead;
    mag_read.data_buffer = NULL;
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
    depth_read.data_size = 3; //24-bit result
    depth_read.device_address = DEPTH_ADDR;
    depth_read.device_id = SID_DEPTH_1; //Modified each time the node is enqueued
    depth_read.mode = READ;
    depth_read.sub_address = 0; //send 0x00 to initiate a depth_read

    temp_read = depth_read;
    temp_read.device_id = SID_TEMP_1;
    
    temp_prep = depth_prep;
    temp_prep.device_id = SID_TEMP_1;
    temp_prep.sub_address = 0x50; //OSR = 256 for temp reading
}

void switchChannel(int channel)
{
    uint8 config_one = 0, config_two = 0;
    switch (channel)
    {
        case 0:
            config_one = 0b1;
            config_two = 0;
            break;
        case 1:
            config_one = 0b10;
            config_two = 0;
            break;
        case 2:
            config_one = 0;
            config_two = 0b01;
            break;
        case 3:
            config_one = 0;
            config_two = 0b10;
            break;
        default:
            config_one = config_two = 0;
            break;
    }
    
    I2C_Node mux1 = {0}, mux2 = {0};
    mux1.callback = NULL;
    mux1.data_buffer = NULL;
    mux1.device_address = MUX_1_ADDR;
    mux1.data_size = 0;
    mux1.device_id = SID_MUX_1;
    mux1.sub_address = config_one; //Sub-address does not exist for the mux chip
    mux1.mode = WRITE;
    
    send_I2C(I2C_CH_1, mux1);
    
    mux2.callback = NULL;
    mux2.data_buffer = NULL;
    mux2.device_address = MUX_2_ADDR;
    mux2.data_size = 0;
    mux2.device_id = SID_MUX_2;
    mux2.sub_address = config_two;
    mux2.mode = WRITE;
    
    send_I2C(I2C_CH_1, mux2);
}