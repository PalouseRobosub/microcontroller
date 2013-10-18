 /********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/


#include "functions.h"
#include "Sensors.h"
#include "I2C_ISR.h"
#include "Testbench.h"
#include "Queue.h"
#include "Timer_ISR.h"
#include "UART_ISR.h"


/*************************************************************************
 Processor Configuration
 ************************************************************************/
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#pragma config ICESEL = ICS_PGx2

 
/*************************************************************************
 Main Function
 ************************************************************************/
int main(void)
{
    int dummy;
    uint Byte1 = 0x41, Byte2 = 0x42, Byte3 = 0x43;

    TRISGbits.TRISG0 = 0;
    TRISGbits.TRISG1 = 0;
    PORTGbits.RG0 = 0;
    PORTGbits.RG1 = 0;
    dummy = 0;

    //setup led pins for write
    setup_leds();
    

    //setup/configure hardware modules
    timer_1_setup();
    i2c_1_setup();

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();


    //start each ISR
    i2c_1_begin();
    uart_setup();
    uart_CreateNode( Byte1, Byte2, Byte3 );
    uart_begin();
    
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();
    while (1)
    {
        //nothing should be in here
        //timer_1_begin();
        //++dummy;
    }

    return 0;
}




