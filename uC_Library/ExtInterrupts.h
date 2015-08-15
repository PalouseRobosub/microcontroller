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
    }Pin;
    
    typedef enum {
        falling,
        rising
    } Polarity;
    
    typedef enum {
        none,
        pullup,
        pulldown
    } Resistor;
    
    typedef struct INTERRUPT_CONFIG{
        Interrupt extInt;
        Pin pin;
        Polarity polarity;
        int enable;
        Resistor resistor;
        void *callback;
    } Interrupt_Config;
    
    void initialize_Interrupt(Interrupt_Config config);
    void disable_Interrupt(Interrupt extInt);
    void enable_Interrupt(Interrupt extInt);
    

#ifdef	__cplusplus
}
#endif

#endif	/* EXTINTERRUPTS_H */

