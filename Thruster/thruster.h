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
    
#define NUM_THRUSTERS 8

    typedef enum
    {
        THRUSTER_ADDR_1 = 0x29,
        THRUSTER_ADDR_2 = 0x29,
        THRUSTER_ADDR_3 = 0x29,
        THRUSTER_ADDR_4 = 0x29,
        THRUSTER_ADDR_5 = 0x29,
        THRUSTER_ADDR_6 = 0x29,
        THRUSTER_ADDR_7 = 0x29,
        THRUSTER_ADDR_8 = 0x29
        
    }THRUSTER_I2C_ADDRESS;
    
    typedef enum
    {
        THRUSTER_P_COMMAND = 0x00
        
    }THRUSTER_PACKET_TYPES;
    
    
    Error read_thrusters(void);
    void init_thrusters(void);
    Error read_thrusters_timer_callback(void);
    void read_thrusters_i2c_callback(I2C_Node);
    void thruster_packetizer_callback(uint8* data, uint8 data_size);
    

#ifdef	__cplusplus
}
#endif

#endif	/* THRUSTERS_H */

