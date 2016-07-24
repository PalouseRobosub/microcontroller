#include "Sensors.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep, temp_prep, temp_read;

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
    mag_read.device_id = SID_MAGNETOMETER_1; //Modified each time the node is enqueued
    mag_read.mode = READ;
    mag_read.sub_address = 0x03;
    
    depth_prep.callback = NULL; //Cant be called
    depth_prep.data_buffer = NULL;
    depth_prep.data_size = 0;
    depth_prep.device_address = DEPTH_ADDR;
    depth_prep.device_id = SID_DEPTH_1; //Modified each time the node is enqueued
    depth_prep.mode = WRITE;
    depth_prep.sub_address = 0x4A; //Command byte to start conversion of depth. Sub-address is not used. OSR = 256
    
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
    temp_prep.sub_address = 0x5A; //OSR = 256 for temp reading
}