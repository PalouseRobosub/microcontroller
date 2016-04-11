#include "Sensors.h"
#include "sublibinal.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep;
extern char read;
char depths_converting = 0;
char sensors_read = 0;
char channel_zero_busy, channel_one_busy;

//Timer callback for reading sensors
void readSensors()
{
    depths_converting = 0;
    sensors_read = 0;
    
    switchChannel(0); //Switch to channel 0 of mux
    channel_zero_busy = 1;
    
    //Send depth conversion start packets
    /*depth_prep.device_id = SID_DEPTH_1;
    send_I2C(I2C_CH_1, depth_prep);
    
    depth_prep.device_id = SID_DEPTH_2;
    send_I2C(I2C_CH_2, depth_prep);*/
    
    gyro_read.device_id = SID_GYROSCOPE_1;
    mag_read.device_id = SID_MAGNOMETER_1;
    accel_read.device_id = SID_ACCELEROMETER_1;
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, mag_read);
    send_I2C(I2C_CH_1, accel_read);
    
    gyro_read.device_id = SID_GYROSCOPE_2;
    mag_read.device_id = SID_MAGNOMETER_2;
    accel_read.device_id = SID_ACCELEROMETER_2;
    send_I2C(I2C_CH_2, gyro_read);
    send_I2C(I2C_CH_2, mag_read);
    send_I2C(I2C_CH_2, accel_read);
    
    while (channel_zero_busy)
        bg_process_I2C();
    
    switchChannel(1); //Switch to channel 1 of mux
    channel_one_busy = 1;
    
    //Send depth conversion start packets
    /*depth_prep.device_id = SID_DEPTH_3;
    send_I2C(I2C_CH_1, depth_prep);
    
    depth_prep.device_id = SID_DEPTH_4;
    send_I2C(I2C_CH_2, depth_prep);*/
    
    gyro_read.device_id = SID_GYROSCOPE_3;
    mag_read.device_id = SID_MAGNOMETER_3;
    accel_read.device_id = SID_ACCELEROMETER_3;
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, mag_read);
    send_I2C(I2C_CH_1, accel_read);
    
    while (channel_one_busy)
        bg_process_I2C();
}

void timeToRead()
{
    read = 1;
}

//Sensor has been read callback function
void sensorRead(I2C_Node node)
{
    uint8 packet[10];
    packet[0] = node.device_id;
    memcpy(&packet[1], node.data_buffer, node.data_size);
    send_packet(PACKET_UART_CH_1, packet, node.data_size + 1);
    
    sensors_read++;
    if (sensors_read == 6)//8 with depth -> 6 without
        channel_zero_busy = 0;
    else if (sensors_read >= 9) //13 with depth -> 13 without
        channel_one_busy = 0;
}

//Callback to say a transaction has completed for a depth conversion start
void depthConverting(I2C_Node node)
{
    depths_converting++;
    if (depths_converting == 2)
    {
        TMR2 = 0;
        enable_Timer(WAIT_TIMER);
        depths_converting = 0;
    }
}

void readDepth()
{
    disable_Timer(WAIT_TIMER);
    if (channel_zero_busy)
    {
        //Read 1 and 2
        depth_read.device_id = SID_DEPTH_1;
        send_I2C(I2C_CH_1, depth_read);
        depth_read.device_id = SID_DEPTH_2;
        send_I2C(I2C_CH_2, depth_read);
    }
    else
    {
        //Read 3 and 4
        depth_read.device_id = SID_DEPTH_3;
        send_I2C(I2C_CH_1, depth_read);
        depth_read.device_id = SID_DEPTH_4;
        send_I2C(I2C_CH_2, depth_read);
    }
}