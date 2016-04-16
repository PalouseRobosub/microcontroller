#include "Sensors.h"
#include "sublibinal.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep;
extern char read;
char depth_ready_1 = 0, depth_ready_2 = 0;
char converting_1 = 0, converting_2 = 0;

//Timer callback for reading sensors
void readSensors()
{
    
    switchChannel(0); //Switch to channel 0 of mux
    /*
    //Send depth conversion start packets
    if (depth_ready_1)
    {
        depth_read.device_id = SID_DEPTH_1;
        send_I2C(I2C_CH_1, depth_read);
        depth_read.device_id = SID_DEPTH_2;
        send_I2C(I2C_CH_2, depth_read);
        depth_ready_1 = 0;
    }
    else if (converting_1 == 0 && !depth_ready_1)
    {
        send_I2C(I2C_CH_1, depth_prep);
        send_I2C(I2C_CH_2, depth_prep);
        enable_Timer(WAIT_TIMER_1);
        converting_1 = 1;
    }*/
    
    gyro_read.device_id = SID_GYROSCOPE_1;
    mag_read.device_id = SID_MAGNOMETER_1;
    accel_read.device_id = SID_ACCELEROMETER_1;
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, mag_read);
    send_I2C(I2C_CH_1, accel_read);
    
    gyro_read.device_id = SID_GYROSCOPE_2;
    mag_read.device_id = SID_MAGNOMETER_2;
    accel_read.device_id = SID_ACCELEROMETER_2;
    //send_I2C(I2C_CH_2, gyro_read);
    //send_I2C(I2C_CH_2, mag_read);
    //send_I2C(I2C_CH_2, accel_read);
    
    switchChannel(1); //Switch to channel 1 of mux
    
    //Send depth conversion start packets
    /*
    if (depth_ready_2)
    {
        depth_read.device_id = SID_DEPTH_3;
        send_I2C(I2C_CH_1, depth_read);
        depth_read.device_id = SID_DEPTH_4;
        send_I2C(I2C_CH_2, depth_read);
        depth_ready_2 = 0;
    }
    else if (converting_2 == 0 && !depth_ready_2)
    {
        //send_I2C(I2C_CH_1, depth_prep);
        //send_I2C(I2C_CH_2, depth_prep);
        enable_Timer(WAIT_TIMER_2);
        converting_2 = 1;
    }*/
    
    gyro_read.device_id = SID_GYROSCOPE_3;
    mag_read.device_id = SID_MAGNOMETER_3;
    accel_read.device_id = SID_ACCELEROMETER_3;
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, mag_read);
    send_I2C(I2C_CH_1, accel_read);
}

void timeToRead()
{
    read = 1;
}

//Sensor has been read callback function
void sensorRead(I2C_Node node)
{
    uint8 packet[10];
    uint8 tmp;
    int i;
    packet[0] = node.device_id;
    memcpy(&packet[1], node.data_buffer, node.data_size);
    
    switch (node.device_id)
    {
        case SID_MAGNOMETER_1:
        case SID_MAGNOMETER_2:
        case SID_MAGNOMETER_3:
        case SID_GYROSCOPE_1:
        case SID_GYROSCOPE_2:
        case SID_GYROSCOPE_3: //Reverse byte order
            for (i = 0; i < node.data_size; i += 2)
            {
                tmp = packet[i+1];
                packet[i+1] = packet[i+2];
                packet[i+2] = tmp;
            }
            break;
        default:
            break;
            
    }
    send_packet(PACKET_UART_CH_1, packet, node.data_size + 1);
}

void readDepth_1()
{
    disable_Timer(WAIT_TIMER_1);
    TMR2 = 0;
    converting_1 = 0;
    depth_ready_1 = 1;
}

void readDepth_2()
{
    disable_Timer(WAIT_TIMER_2);
    TMR3 = 0;
    converting_2 = 0;
    depth_ready_2 = 1;
}