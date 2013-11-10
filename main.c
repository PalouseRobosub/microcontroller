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
#include "comm_UART_ISR.h"
#include "motor_UART_ISR.h"


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
    AD1PCFG = 0xFFFF;
//    TRISGbits.TRISG0 = 0;
//    TRISGbits.TRISG1 = 0;
//    PORTGbits.RG0 = 0;
//    PORTGbits.RG1 = 0;
    dummy = 0;

    //setup led pins for write
    setup_leds();

    TRISAbits.TRISA3 = 0; //Set LED4 as output
    PORTAbits.RA3 = 0; //Turn off LED4

    TRISCbits.TRISC1 = 0; //Set LED5 as output
    PORTCbits.RC1 = 0; //Turn off LED5
    

    //setup/configure hardware modules
    timer_1_setup();
    i2c_bank_0_setup();
    comm_uart_setup();
    motor_uart_setup();

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();
    i2c_GYRO_Initialize();


    //start each ISR
    i2c_bank_0_begin();
    
    comm_uart_CreateNode( Byte1, Byte2, Byte3 );
    //Motor1_Backward( 128, 50 );
    
    motor_uart_begin();
    comm_uart_begin();
    
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




