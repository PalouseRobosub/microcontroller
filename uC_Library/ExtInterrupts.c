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
        case INT1:
            if ((config.pin - (3*8)) > 0b1000) {
                config.pin = 3*8;
            }
            INT1R = config.pin - (3*8);
            break;
        case INT2:
            if ((config.pin - (2*8)) > 0b1000) {
                config.pin = 2*8;
            }
            INT2R = config.pin - (3*8);
            break;
        case INT3:
            if ((config.pin -(1*8)) > 0b1000) {
                config.pin = 1*8;
            }
            INT3R = config.pin - (3*8);
            break;
        case INT4:
            if (config.pin > 0b1000) {
                config.pin = 0;
            }
            INT4R = config.pin;
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