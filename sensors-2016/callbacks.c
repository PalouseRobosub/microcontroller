#include "Sensors.h"

uint16 configurations[4][6];
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
    static uint8 temp = TRUE;
    static uint8 cnt = 1;
    
    if (contended) return;
    
    calls++;
    
    /*
     * On the first read, we don't quite have enough time to get a valid sample.
     */
    if (cnt > 0)
    {
        temp_read_temp_prep_task();
        cnt--;
    }
    else if (temp == TRUE)
    {
        temp_read_depth_prep_task();
        temp = FALSE;
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
    static uint8 message[45];
    uint8 cursor = 0;
    unsigned int timestamp;
    int i;
    
    TMR3 = 0; //Reset the reset timer count
    get_data_I2C(&node, data_registers[node.device_id]);
    
    /*
     * Store the data in internal registers until we are ready to send it.
     * 
     * If we are currently at the end of a device set, send data over UART.
     */
    timestamp = TMR4;
    switch (node.device_id)
    {
        case SID_ACCELEROMETER_3:
            message[cursor++] = GYROSCOPE_ACCELEROMETER_DATA;
            message[cursor++] = timestamp >> 8 & 0xFF;
            message[cursor++] = timestamp & 0xFF;
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
            message[cursor++] = timestamp >> 8 & 0xFF;
            message[cursor++] = timestamp & 0xFF;
            for (i = SID_MAGNETOMETER_1; i <= SID_MAGNETOMETER_3; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 6);
                cursor += 6;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
        case SID_DEPTH_4:
            message[cursor++] = DEPTH_DATA;
            message[cursor++] = timestamp >> 8 & 0xFF;
            message[cursor++] = timestamp & 0xFF;
            for (i = SID_DEPTH_1; i <= SID_DEPTH_4; ++i)
            {
                memcpy(&message[cursor], data_registers[i], 3);
                cursor += 3;
            }
            send_packet(PACKET_UART_CH_1, message, cursor);
            break;
            
        case SID_TEMP_4:
            message[cursor++] = TEMP_DATA;
            message[cursor++] = timestamp >> 8 & 0xFF;
            message[cursor++] = timestamp & 0xFF;
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

void configRead(I2C_Node node)
{
    get_data_I2C(&node, &(configurations[(node.device_id - SID_DEPTH_CON_1_1) / 6][node.device_id%6]));
}

void packetizer_callback(uint8 *data, uint8 len)
{
    uint8 cursor = 0;
    int i;
    uint8 message[49];
    if (*data == RESET_COMMAND)
    {
        disable_Interrupts();

        /*
         * Disable timers.
         */
        disable_Timer(GYRO_ACCEL_TIMER);
        disable_Timer(MAG_TIMER);
        disable_Timer(RESET_TIMER);
        disable_Timer(DEPTH_TIMER);

        
        LATBbits.LATB15 = 1; //I2C is reset.
        flush_queue_I2C(I2C_CH_1);
        configureSensors();
        LATBbits.LATB15 = 0; //I2C is not reset.
        for (i = 0; i < 1500000; ++i);

        /*
         * Enable timers.
         */
        enable_Timer(GYRO_ACCEL_TIMER);
        enable_Timer(MAG_TIMER);
        enable_Timer(RESET_TIMER);
        enable_Timer(DEPTH_TIMER);
        enable_Interrupts();
    }
    else if (*data == GET_EEPROM_COMMAND)
    {
        message[cursor++] = DEPTH_CONFIG_DATA;
        for (i = 0; i < 4; ++i)
        {
            memcpy(&message[cursor], configurations[i], 12);
            cursor += 12;
        }
        send_packet(PACKET_UART_CH_1, message, cursor);
    }
    else if (*data == QUERY_DEPTH_CONFIG_COMMAND)
    {
        set_mux(MUX_1_ADDR, channel_one);
        set_mux(MUX_2_ADDR, channel_none);
        queryDepthConfig(0);
        set_mux(MUX_1_ADDR, channel_two);
        queryDepthConfig(1);
        set_mux(MUX_1_ADDR, channel_none);
        set_mux(MUX_2_ADDR, channel_one);
        queryDepthConfig(2);
        set_mux(MUX_2_ADDR, channel_two);
        queryDepthConfig(3);
    }
}
