/* 
 * File:   thruster.h
 * Author: james
 *
 * 
 */

#ifndef THRUSTERS_H
#define	THRUSTERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"
#include "I2C.h"
#include "packetizer.h"
    
    //SPECIAL CONSTANTS
    #define THROTTLE_REG_ADDR 0x00     //address of of the first throttle control register
    #define THROTTLE_REG_SIZE 2        //number of bytes for a throttle size
    #define SENSOR_REG_SIZE 2          //number of bytes for a throttle size
    #define SENSOR_REG_START_ADDR 0x02 //address of first sensor register
    #define NUM_SENSOR_REG 8           //number of sensor registers
    #define ALIVE_REG_ADDR 0x0A        //address of the isAlive? register
    #define ALIVE_VALUE 0xAB           //value that should be in isAlive? register
    
    typedef enum
    {
        CMD_THROTTLE = 0x00,
        CMD_READ_SENSOR = 0x01,
        CMD_READ_ALL_SENSORS = 0x02,
        CMD_CHECK_ALIVE = 0x03
    }THRUSTER_CMD_ID;
    
    typedef enum
    {
        RESP_READ_SENSOR = CMD_READ_SENSOR,
        RESP_READ_ALL_SENSORS = CMD_READ_ALL_SENSORS,
        RESP_CHECK_ALIVE = CMD_CHECK_ALIVE
        
    }THRUSTER_REPONSE_ID;
    
    void CMD_READ_SENSOR_i2c_callback(I2C_Node node);
    void CMD_READ_ALL_SENSORS_i2c_callback(I2C_Node node);
    void CMD_CHECK_ALIVE_i2c_callback(I2C_Node node);
    void thruster_packetizer_callback(uint8* data, uint8 data_size);
    

#ifdef	__cplusplus
}
#endif

#endif	/* THRUSTERS_H */

