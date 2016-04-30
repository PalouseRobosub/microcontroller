#include "Sensors.h"
#include "sublibinal.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep, temp_prep, temp_read;
extern char read;

extern uint8 depthConfigurations[4][6][2];

char read_temp = 1, temp_prepped = 0; //Initialize read_temp to 1 to get an initial read

//Timer callback for reading sensors
void readSensors(int channel)
{
    switchChannel(channel); //Switch to channel of mux
    
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

int cnt = 0;
void readTemperature()
{
    cnt++;
    if (cnt >= 1) //Read temperature every 10 triggers of this interrupt
    {
        read_temp = 1;
        cnt = 0;
    }
}

void timeToRead()
{
    read = 1;
}

//Sensor has been read callback function
void sensorRead(I2C_Node node)
{
    TMR3 = 0; //Reset the reset timer count
    
    uint8 packet[10];
    uint8 tmp;
    int i;
    packet[0] = node.device_id;
    get_data_I2C(&node, &packet[1]);
    
    if (node.device_id > SID_DEPTH_CON_1_1)
    {
        i = node.device_id - SID_DEPTH_CON_1_1;
        //Store into configuration structure for querying later as well
        depthConfigurations[i/6][i%6][0] = packet[1];
        depthConfigurations[i/6][i%6][1] = packet[2];
    }
    
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
        case SID_TEMP_1:
        case SID_TEMP_2:
        case SID_TEMP_3:
        case SID_TEMP_4: 
        case SID_DEPTH_1:
        case SID_DEPTH_2:
        case SID_DEPTH_3:
        case SID_DEPTH_4:
            if (!read_temp)
            {
                for ( i = 0; i < 4; i++)
                {
                    depth_prep.device_id = SID_DEPTH_1 + i;
                    if (i == 3)
                    {
                        switchChannel(i);
                        send_I2C(I2C_CH_1, depth_prep);
                    }
                }
            }
            else
            {
                for ( i = 0; i < 4; i++)
                {
                    temp_prep.device_id = SID_TEMP_1 + i;
                    if (i == 3)
                    {
                        switchChannel(i);
                        send_I2C(I2C_CH_1, temp_prep);
                    }
                }
                temp_prepped = 1;
            }
            enable_Timer(WAIT_TIMER);
            break;
        default:
            break;
            
    }
    send_packet(PACKET_UART_CH_1, packet, node.data_size + 1);
}

void readDepth()
{
    LATAINV |= 1<<3;
    int i = 0;
    
    disable_Timer(WAIT_TIMER);
    TMR2 = 0;
    
    if (temp_prepped)
    {
        //Enqueue a temp read
        for (i = 0; i < 4; i++)
        {
            temp_read.device_id = SID_TEMP_1 + i;
            if (i == 3)
            {
                switchChannel(i);
                send_I2C(I2C_CH_1, temp_read);
            }
        }
        read_temp = 0; //Temperature reads have been enqueued
        temp_prepped = 0; //Temperature read is enqueued. reset.
    }
    else
    {
        //Enqueue depth reads
        for (i = 0; i < 4; i++)
        {
            depth_read.device_id = SID_DEPTH_1 + i;
            if (i == 3)
            {
                switchChannel(i);
                send_I2C(I2C_CH_1, depth_read);
            }
        }
    }
}