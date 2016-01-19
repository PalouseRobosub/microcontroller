#include "Bootloader.h"

extern Frame RX, TX;

void initialize_UART_Interface(int baudRate, int pbclk)
{
    ANSELBbits.ANSB13 = 0;
    ANSELBbits.ANSB15 = 0;
    TRISBbits.TRISB13 = 1; //Input -> RX
    TRISBbits.TRISB15 = 0; //Output -> TX
    
    U1RXR = 3; //U1RX
    RPB15R = 1; //U1TX
    
    U1STA = U1MODE = 0;
    U1BRG = (pbclk/16/baudRate-1);
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1; //Turn the UART on
}

void UART_tasks()
{
    int i;
    
    uint8_t byte;
	BYTE dummy;

	if(U1STA & 0x000E)              // receive errors?
	{
		dummy = U1RXREG; 			// dummy read to clear FERR/PERR
		U1STAbits.OERR = 0;			// clear OERR to keep receiving
	}
    if (U1STAbits.URXDA)
    {
        byte = U1RXREG;
        constructRXFrame(byte);
    }
    
    if (TX.isValid)
    {
        for (i = 0; i < TX.length; i++)
        {
            send_Byte(TX.buffer[i]);
        }
        TX.length = 0;
        TX.isValid = 0;
    }
}

uint8_t get_Byte()
{
    uint8_t byte;
    while (U1STAbits.URXDA == 0);
    byte = U1RXREG;
    return byte;
}

void send_Byte(uint8_t byte)
{
    while(U1STAbits.UTXBF); // wait for TX buffer to be empty
    U1TXREG = byte;
}