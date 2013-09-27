/*
 * File:   main.c
 * Author: James
 *
 * Created on September 24, 2013, 4:18 PM
 */

#include <p32xxxx.h>
#include <peripheral/i2c.h>
#include <peripheral/timer.h>
//#include <peripheral/ports.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ  (80000000)
#define I2C_FREQ  (100000)

#define I2C_BUS I2C1
#define I2C_ADDR 0x17

typedef unsigned int uint;
typedef unsigned char uint8;


void delay(void)
{
    uint i;
    for (i=0; i<10000; ++i);
    return;
}

void __ISR(_TIMER_1_VECTOR,ipl3) Timer1Handler(void)
{
//... perform application specific operations in response to the interrupt
//IFS0CLR = 0x00000010; // Be sure to clear the Timer1 interrupt status
    IFS0bits.T1IF = 0;
}


// void __ISR(_TIMER_1_VECTOR, ipl2) _Timer1Handler(void)
// {
// PORTGbits.RG1 = !PORTGbits.RG1;
// mT1ClearIntFlag(); // clear the interrupt flag
// return;
// }

int main(void) {
   // configure the core timer roll-over rate (100msec)
     OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0x0FF);

//     // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
//     ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

     IEC0bits.T1IE = 1;
     IPC1bits.T1IP = 3;

     // enable device multi-vector interrupts
     INTEnableSystemMultiVectoredInt();
//     INTCONbits.MVEC = 1;
//     INTCONSET = _INTCON_MVEC_MASK;



// T2CON = 0x0; // Stop the timer and clear the control register,
//// prescaler at 1:1,internal clock source
//TMR2 = 0x0; // Clear the timer register
//PR2 = 0xFFFF; // Load the period register
//IPC2SET = 0x0000000C; // Set priority level = 3
//IPC2SET = 0x00000001; // Set subpriority level = 1
//// Can be done in a single operation by assigning PC2SET = 0x0000000D
//IFS0CLR = 0x00000100; // Clear the timer interrupt status flag
//IEC0SET = 0x00000100; // Enable timer interrupts
//T2CONSET = 0x8000; // Start the timer

    TRISGbits.TRISG0 = 0;
    PORTGbits.RG0 = 0;
    TRISGbits.TRISG1 = 0;
    PORTGbits.RG1 = 1;



    while (1)
    {
       // delay();
        PORTGbits.RG0 = !PORTGbits.RG0;
        PORTGbits.RG0 = 0;
        PORTGbits.RG0 = 0;
        PORTGbits.RG0 = 0;
        IFS0bits.T1IF = 1;
    }


    return 0;
}





