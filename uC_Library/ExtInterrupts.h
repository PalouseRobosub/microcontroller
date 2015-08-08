/* 
 * File:   ExtInterrupts.h
 * Author: Ryan
 *
 * Created on August 6, 2015, 5:42 PM
 */

#ifndef EXTINTERRUPTS_H
#define	EXTINTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef int Ext_Int;

    typedef enum {
        INT0,
        INT1,
        INT2,
        INT3,
        INT4
    }Interrupt;

    typedef enum {
        RPA0, // Start Int4
        RPB3,
        RPB4,
        RPB15,
        RPB7,
        RPC7,
        RPC0,
        RPC5, // End Int4
        RPA1, //Start Int3
        RPB5,
        RPB1,
        RPB11,
        RPB8,
        RPA8,
        RPC8,
        RPA9, //End Int3
        RPA2, //Start Int2
        RPB6,
        RPA4,
        RPB13,
        RPB2,
        RPC6,
        RPC1,
        RPC3, // End Int2
        RPA3, //Start Int1
        RPB14,
        RPB0,
        RPB10,
        RPB9,
        RPC9,
        RPC2,
        RPC4, // End Int1
    }Pin;
    
    typedef enum {
        falling,
        rising
    } Polarity;
    
    typedef struct INTERRUPT_CONFIG{
        Interrupt extInt;
        Pin pin;
        Polarity polarity;
        int priority;
        int subPriority;
        bool enable;
        void *callback;
    } Interrupt_Config;
    
    void initialize_INT(Interrupt_Config config);
    void disable_INT(Interrupt extInt);
    void enable_INT(Interrupt extInt);

#ifdef	__cplusplus
}
#endif

#endif	/* EXTINTERRUPTS_H */

