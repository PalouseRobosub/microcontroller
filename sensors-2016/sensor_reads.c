#include "Sensors.h"
#include "sublibinal.h"

I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep, temp_prep, temp_read;
extern char read;

extern uint8 depthConfigurations[4][6][2];

char read_temp = 1, temp_prepped = 0; //Initialize read_temp to 1 to get an initial read
char prep_sent = 0, prepped = 0, send_prep = 0;

//Timer callback for reading sensors
void readSensors(int channel)
{
    int i = 0;
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
    
    if (channel == 3)
    {
        //Enqueue depth read
        if (prepped)
        {
            if (temp_prepped)
            {
                temp_read.device_id = SID_TEMP_4;
                send_I2C(I2C_CH_1, temp_read);
                temp_prepped = 0;
            }
            else 
            {
                depth_read.device_id = SID_DEPTH_4;
                send_I2C(I2C_CH_1, depth_read);
            }
            prepped = 0;
        }
        else if (send_prep)
        {
            send_I2C(I2C_CH_1, depth_prep); //SID for write is irrelevant
            send_prep = 0;
            prep_sent = 1;
            temp_prepped = 0;
        }
        
    }
}

int cnt = 0;
void readTemperature()
{
    cnt++;
    if (cnt >= 10) //Read temperature every 10 triggers of this interrupt
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
                temp_prepped = 0;
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
                read_temp = 0;
            }
            prep_sent = 1;
            TMR2 = 0;
            break;
        default:
            break;
            
    }
    send_packet(PACKET_UART_CH_1, packet, node.data_size + 1);
}

char no_prep_cnt = 0;
void readDepth()
{
    int i = 0;
    
    if (prep_sent)
    {
        prepped = 1;
        prep_sent = 0;
        no_prep_cnt = 0;
    }
    else 
    {
        no_prep_cnt++;
        if (no_prep_cnt >= 10)
        {
            send_prep = 1;
            no_prep_cnt = 0;
        }
    }
}