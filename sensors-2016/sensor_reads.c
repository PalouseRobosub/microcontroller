#include "Sensors.h"
#include "sublibinal.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep;
extern char read;
char depth_ready_1 = 0, depth_ready_2 = 0;
char converting_1 = 0, converting_2 = 0;
char sensors_read = 0;

//Timer callback for reading sensors
void readSensors(int channel)
{
    switchChannel(channel); //Switch to channel of mux
    
    //Send depth configuration packet
    depth_prep.device_id = SID_DEPTH_1 + channel;
    //send_I2C(I2C_CH_1, depth_prep);
    
    //if (channel == 0) 
        //enable_Timer(WAIT_TIMER);
    
    if (channel < 3)
    {
        gyro_read.device_id = SID_GYROSCOPE_1 + channel;
        mag_read.device_id = SID_MAGNOMETER_1 + channel;
        accel_read.device_id = SID_ACCELEROMETER_1 + channel;
        send_I2C(I2C_CH_1, gyro_read);
        send_I2C(I2C_CH_1, mag_read);
        send_I2C(I2C_CH_1, accel_read);
    }
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
    get_data_I2C(&node, &packet[1]);
    
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

void readDepth()
{
    int i = 0;
    disable_Timer(WAIT_TIMER);
    TMR2 = 0;
    
    //Enqueue depth reads
    for (i = 0; i < 4; i++)
    {
        switchChannel(i);
        depth_read.device_id = SID_DEPTH_1 + i;
        send_I2C(I2C_CH_1, depth_read);
    }
}