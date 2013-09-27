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
    i2c_1_setup();


    //start each ISR
    i2c_1_begin();
    
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    while (1)
    {
        //nothing should be in here
    }

    return 0;
}




