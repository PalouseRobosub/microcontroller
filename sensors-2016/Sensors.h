#ifndef SENSORS_H
#define SENSORS_H

#include <string.h>
#include "sublibinal.h"

//Define all addresses for Sensors
#define ACCEL_ADDR 0x53
#define GYRO_ADDR 0x68
#define DEPTH_ADDR 0x76
#define MAG_ADDR 0x1E
#define MUX_ADDR 0x70

typedef enum {
    SID_ACCELEROMETER_1,
    SID_ACCELEROMETER_2,
    SID_ACCELEROMETER_3,
    SID_GYROSCOPE_1,
    SID_GYROSCOPE_2,
    SID_GYROSCOPE_3,
    SID_MAGNOMETER_1,
    SID_MAGNOMETER_2,
    SID_MAGNOMETER_3,
    SID_DEPTH_1,
    SID_DEPTH_2,
    SID_DEPTH_3,
    SID_DEPTH_4,
    SID_MUX_1,
    SID_MUX_2
} Sensor_ID;


//General definitions
#define CONTROL_BYTE 0x0A //packetizer control byte
#define UART_SPEED 115200 //115.2k baud rate
#define PB_CLK 15000000 //15MHz
#define READ_RATE 100 //Read the sensors at 100Hz


#define READ_TIMER Timer_1 //Utilizing timer 1 as our sensor timer
#define WAIT_TIMER Timer_2 //Timer 2 is utilized for waitng 20ms for depth conversions to complete
    //If wait timer is changed, ALSO must change TMRx = 0 in sensor_reads.c:depthConverting callback

//Function prototypes

//Peripheral_Config.h
void configureTimer();
void configureSerial();
void configureI2C();

//Sensor_Reads.c
void readSensors(); //Enqueue reads for all of our sensors
void sensorRead(I2C_Node node); //Sensor read node callback completed
void depthConverting(I2C_Node node); //Depth conversion configuration node callback
void timeToRead(); //Read timer callback -> sets read = 1
void readDepth(); //Callback once conversion is complete to read the depth sensors

//Sensor_Calibration.c
void config_done(I2C_Node node); //Callback for configuration nodes
void configureSensors(); //Configuration for all sensors - this will block until all calibration is completed
void configureAccelerometer(int channel); //Configure accelerometers on channel 0/1
void configureGyroscope(int channel); //Configure gyroscopes on channel 0/1
void configureMagnometers(int channel); //Configure mangometers on channel 0/1


//Sensor_Support.c
void switchChannel(int channel); //Switch to specified channel - will block until channel is switched
void switchComplete(I2C_Node node); //Callback function for switch nodes
void configureReadNodes(); //Set up the values of the read nodes once

#endif