#ifndef SENSORS_H
#define SENSORS_H

#include <string.h>
#include "sublibinal.h"

//Define all addresses for Sensors
#define ACCEL_ADDR 0x53
#define GYRO_ADDR 0b1101000
#define DEPTH_ADDR 0x76
#define MAG_ADDR 0x1E
#define MUX_2_ADDR 0b1110000
#define MUX_1_ADDR 0b1110001

typedef enum {
    SID_GYROSCOPE_1,
    SID_GYROSCOPE_2,
    SID_GYROSCOPE_3,
    SID_ACCELEROMETER_1,
    SID_ACCELEROMETER_2,
    SID_ACCELEROMETER_3,
    SID_MAGNETOMETER_1,
    SID_MAGNETOMETER_2,
    SID_MAGNETOMETER_3,
    SID_DEPTH_1,
    SID_DEPTH_2,
    SID_DEPTH_3,
    SID_DEPTH_4,
    SID_TEMP_1,
    SID_TEMP_2,
    SID_TEMP_3,
    SID_TEMP_4,
    SID_MUX_1,
    SID_MUX_2,
    SID_DEPTH_CON_1_1,
    SID_DEPTH_CON_1_2,
    SID_DEPTH_CON_1_3,
    SID_DEPTH_CON_1_4,
    SID_DEPTH_CON_1_5,
    SID_DEPTH_CON_1_6,
    SID_DEPTH_CON_2_1,
    SID_DEPTH_CON_2_2,
    SID_DEPTH_CON_2_3,
    SID_DEPTH_CON_2_4,
    SID_DEPTH_CON_2_5,
    SID_DEPTH_CON_2_6,
    SID_DEPTH_CON_3_1,
    SID_DEPTH_CON_3_2,
    SID_DEPTH_CON_3_3,
    SID_DEPTH_CON_3_4,
    SID_DEPTH_CON_3_5,
    SID_DEPTH_CON_3_6,
    SID_DEPTH_CON_4_1,
    SID_DEPTH_CON_4_2,
    SID_DEPTH_CON_4_3,
    SID_DEPTH_CON_4_4,
    SID_DEPTH_CON_4_5,
    SID_DEPTH_CON_4_6
} Sensor_ID;

typedef enum {
    GYROSCOPE_ACCELEROMETER_DATA,
    MAGNETOMETER_DATA,
    DEPTH_DATA,
    TEMP_DATA
} Data_Packet_Types;

typedef enum {
    channel_none,
    channel_one,
    channel_two
} Mux_Channel;

/*
 * Peripheral configuration definitions.
 */
#define CONTROL_BYTE 0x0A //packetizer control byte
#define UART_SPEED 115200 //115.2k baud rate
#define PB_CLK 15000000 //15MHz
#define BUFF_SIZE sizeof(I2C_Node) * 50 //I2C Node buffer size
#define UART_BUFF_SIZE 512
#define DATA_BUFF_SIZE 1024 //I2C Data buffer size

/*
 * These outline what timers are used for what purpose.
 */
#define DEPTH_TIMER Timer_1
#define MAG_TIMER Timer_2
#define RESET_TIMER Timer_3 //If this is changed, also change it in sensor read callback!
#define SCK_RST_TIMER RESET_TIMER
#define TIMESTAMP_TIMER Timer_4
#define GYRO_ACCEL_TIMER Timer_5

/*
 * These outline the rates at which sensors are read.
 */
#define DEPTH_FREQUENCY 40
#define MAG_FREQUENCY 75
#define GYRO_ACCEL_FREQUENCY 100

//callbacks.c
void readDepth();
void readMag();
void readGyroAccel();
void sensorRead(I2C_Node node);

//peripheral_config.c
void configureTimer();
void configureSerial();
void configureI2C();
void start_scl_reset();
void start_scl_watch();

//sensor_calibration.c
void configureSensors(); 
void configureAccelerometer(int channel); 
void configureGyroscope(int channel); 
void configureMagnometer(int channel); 
void configureDepth(int channel); 

//sensor_support.c
void configureReadNodes(); //Set up the values of the read nodes once

//i2c_correction.c
void reset();
void toggle();

#endif