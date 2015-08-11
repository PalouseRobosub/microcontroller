#include "ExtInterrupts.h"
#include "System.h"
void (*callback_int0) (void);
void (*callback_int1) (void);
void (*callback_int2) (void);
void (*callback_int3) (void);
void (*callback_int4) (void);


void initialize_INT(Interrupt_Config config) 
{
    

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