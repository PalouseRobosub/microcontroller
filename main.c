 /********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/


#include "functions.h"
#include "I2C_ISR.h"
#include "Testbench.h"
#include "Queue.h"
#include "Timer_ISR.h"

/*************************************************************************
 Processor Configuration
 ************************************************************************/
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

 
/*************************************************************************
 Main Function
 ************************************************************************/
int main(void)
{
    //setup/configure hardware modules
    timer_1_setup();
    i2c_1_setup();
    i2c_ACL_Initialize();


    //start each ISR
    i2c_1_begin();
    
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    while (1)
    {
        //nothing should be in here
        //timer_1_begin();
    }

    return 0;
}




