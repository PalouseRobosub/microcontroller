#include "ExtInterrupts.h"
#include "System.h"
void (*callback_int0) (void);
void (*callback_int1) (void);
void (*callback_int2) (void);
void (*callback_int3) (void);
void (*callback_int4) (void);


void initialize_INT(Interrupt_Config config) 
{

    //for initialization, we must first set the directionality of the pin and disable any analog values on the pin
    switch (config.extInt) {
    //set the tristate and the PPS
    //disable analog on this pin
        case INT0:
            //we will disable the analog on this pin and configure the tristate as an input
            TRISBbits.TRISB7 = 1;
            //no analog
            INTCONbits.INT0EP = config.polarity;
            break;
        case INT1:
            INT1R = config.pin - (3*8);
            switch (config.pin) {
                case Pin_RPB14:
                    break;
                case Pin_RPB0:
                    break;
                case Pin_RPB10:
                    break;

                case Pin_RPB9:
                    break;
                case Pin_RPC9:
                    break;
                case Pin_RPC2:
                    break;
                case Pin_RPC4:
                    break;
                default: //RPA3 will also default to here
                    //we will set RPA3 as the pin

                    break;
            }
            INTCONbits.INT1EP = config.polarity;
            break;
        case INT2:
            INT2R = config.pin - (2*8);
            INTCONbits.INT2EP = config.polarity;
            break;
        case INT3:
            INT3R = config.pin - (1*8);
            INTCONbits.INT3EP = config.polarity;
            break;
        case INT4:
            INT4R = config.pin;
            INTCONbits.INT4EP = config.polarity;
            break;
        default:
            //error
            break;
    }

    //next, configure the polarity


    //set the priority

    //enable/disable the interrupt


    //finally, we must set the callback

}
void disable_INT(Interrupt extInt) {
    //clear the bit
    switch (extInt) {
        case INT0:
            break;
        case INT1:
            break;
        case INT2:
            break;
        case INT3:
            break;
        case INT4:
            break;
        default:
            break;
    }
}
void enable_INT(Interrupt extInt) {
    //set the bit
}
/*
void __ISR(_EXTERNAL_0_VECTOR, IPL7AUTO, IPL6AUTO, IPL5AUTO, IPL4AUTO, IPL3AUTO, IPL2AUTO, IPL1AUTO) Ext_Int1_Handler {

    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int0 != NULL) {
        (*callback_int0)();
    }
    
    
    asm volatile ("ei"); //reenable interrupts
}*/