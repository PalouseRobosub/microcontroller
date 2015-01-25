
/********************************************************
 *   File Name: uart.c
 *
 *   Description:
 *              source code file for the uart module
 *
 *
 *********************************************************/


/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "uart.h"

/*************************************************************************
 Variables
 ************************************************************************/


/********************************************************
 *   Function Name: uart_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void uart_setup(void) {
    int baud_rate;

    baud_rate = 9600;
    
    U1BRG = 15E6/(16*baud_rate) - 1; //configure baud rate
    
    U1MODEbits.PDSEL = 0; //select 8 bit format, no parity
    IEC1bits.U1TXIE = 0; //enable the Tx interrupt
    IPC8bits.U1IP = 7;  //set Tx interrrupt priority
    U1STAbits.UTXISEL = 2; //fire when tx buffer is empty
    U1STAbits.UTXEN = 1; //enable transmitter
    U1MODEbits.ON = 1; //enable uart module
    RPB15R = 1; //configure uart 1 to be on RPB15 pin

}



/********************************************************
 *   Function Name: uart_handler()
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_UART_1_VECTOR, IPL7AUTO) uart_handler(void) {

    INTDisableInterrupts();

    


    IFS1bits.U1TXIF = 0; //clear the interrupt flag
    INTEnableInterrupts();

}
