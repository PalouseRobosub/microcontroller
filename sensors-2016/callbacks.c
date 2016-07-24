#include "Sensors.h"

extern uint8 contended;

/**
 * Reads the depth sensors.
 * 
 * @note This function will return if the bus is registered as contended.
 * 
 * @note Every 16 seconds, this will instead read the temperature.
 * 
 * @return None.
 */
void readDepth()
{
    static int calls = 0;
    static uint8 temp = FALSE;
    
    if (contended) return;
    
    calls++;
    
    if (temp == TRUE)
    {
        temp_read_depth_prep_task();
    }
    else if (calls%(DEPTH_FREQUENCY<<4) == 0)
    {
        depth_read_temp_prep_task();
        temp = TRUE;
        calls = 0;
    }
    else
    {
        depth_read_depth_prep_task();
    }
}

/**
 * Reads the magnetometer.
 * 
 * @note This function will return if the bus is registered as contended.
 * 
 * @return None.
 */
void readMag()
{
    if (contended) return;
    magnetometer_read_task();
}

/**
 * Read the accelerometers and gyroscopes.
 * 
 * @note This function will return if the bus is registered as contended.
 * 
 * @return None.
 */
void readGyroAccel()
{
    if (contended) return;
    gyroscope_accelerometer_read_task();
}

/**
 * Callback for completed I2C read.
 * 
 * @note This function will send all data of a specific type when it detects
 *       completion of the last node of a type. Data is transmitted up the UART.
 * 
 * @return None.
 */
void sensorRead(I2C_Node node)
{
    static uint8 data_registers[SID_TEMP_4 + 1][8];
    static uint8 message[43];
    uint8 cursor = 0;
    int i;
    
    TMR3 = 0; //Reset the reset timer count
    
    memcpy(data_registers[node.device_id], node.data_buffer, node.data_size);
    
    /*
     * Store the data in internal registers until we are ready to send it.
     * 
     * If we are currently at the end of a device set, send data over UART.
     */
    switch (node.device_id)
    {
        case SID_ACCELEROMETER_3:
            message[cursor++] = GYROSCOPE_ACCELEROMETER_DATA;
            for (i = SID_GYROSCOPE_1; i <= SID_GYROSCOPE_3; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 8);
                cursor += 8;
            }
            for (i = SID_ACCELEROMETER_1; i <= SID_ACCELEROMETER_3; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 6);
                cursor += 6;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
            
        case SID_MAGNETOMETER_3:
            message[cursor++] = MAGNETOMETER_DATA;
            for (i = SID_MAGNETOMETER_1; i <= SID_MAGNETOMETER_3; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 6);
                cursor += 6;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
        case SID_DEPTH_4:
            message[cursor++] = DEPTH_DATA;
            for (i = SID_DEPTH_1; i <= SID_DEPTH_4; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 3);
                cursor += 3;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
            
        case SID_TEMP_4:
            message[cursor++] = TEMP_DATA;
            for (i = SID_TEMP_1; i <= SID_TEMP_4; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 3);
                cursor += 3;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
            
        default:
            break;
    }
    
}