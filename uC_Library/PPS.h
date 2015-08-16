/* 
 * File:   PPS.h
 * Author: ryan
 *
 * Created on August 16, 2015, 1:24 PM
 */

#ifndef PPS_H
#define	PPS_H

#ifdef	__cplusplus
extern "C" {
#endif


    typedef enum {
        Pin_RPA0, // Start Int4
        Pin_RPB3,
        Pin_RPB4,
        Pin_RPB15,
        Pin_RPB7,// End Int4
        Pin_RPA1, //Start Int3
        Pin_RPB5,
        Pin_RPB1,
        Pin_RPB11,
        Pin_RPB8,//End Int3
        Pin_RPA2, //Start Int2
        Pin_RPA4,
        Pin_RPB13,
        Pin_RPB2,// End Int2
        Pin_RPA3, //Start Int1
        Pin_RPB14,
        Pin_RPB0,
        Pin_RPB10,
        Pin_RPB9,// End Int1
    }Pin_Input;
    
    typedef enum {
        Pin_RPA0,
        Pin_RPA1,
        Pin_RPA2,
        Pin_RPA3,
        Pin_RPB0,
        Pin_RPB1,
        Pin_RPB2,
        Pin_RPB3,
        Pin_RPB4,
        Pin_RPB5,
        Pin_RPB6,
        Pin_RPB7,
        Pin_RPB8,
        Pin_RPB9,
        Pin_RPB10,
        Pin_RPB11,
        Pin_RPB13,
        Pin_RPB14,
        Pin_RPB15
    }Pin_Output;


#ifdef	__cplusplus
}
#endif

#endif	/* PPS_H */

