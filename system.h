/* 
 * File:   system.h
 * Author: James
 *
 * Created on October 29, 2013, 5:48 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H


/*************************************************************************
 System Includes
 ************************************************************************/
#include <peripheral/ports.h>
#include <peripheral/uart.h>

/*************************************************************************
 Chip-Specific Defines
 ************************************************************************/
#if defined(__32MX460F512L__)

//comm_uart defines
#define COMM_UART UART1

#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_PDSEL U1MODEbits.PDSEL //parity and data selection bits
#define COMM_UART_UTXISEL U1STAbits.UTXISEL //tx interrupt selection bits
#define COMM_UART_UTXEN U1STAbits.UTXEN //tx enable
#define COMM_UART_URXEN U1STAbits.URXEN //rx enable
#define COMM_UART_ON U1MODEbits.ON //uart enable
#define COMM_UART_TX_INT_set(x) IEC0SET = (x << 28) //enables or disables the tx interrupt
#define COMM_UART_RX_INT_set(x) IEC0SET = (x << 27) //enablse or disables the rx interrupt
#define COMM_UART_INT_PRIORITY_set(x) IPC6SET = (x << 2) //sets the priority of the uart interrupts
#define _COMM_UART_VECTOR _UART1_VECTOR //interrupt vector
#define COMM_UART_RXIF IFS0bits.U1RXIF //Rx interrupt flag
#define COMM_UART_TXIF IFS0bits.U1TXIF //Tx interrupt flag
#define COMM_UART_RXREG U1RXREG //Rx register
#define COMM_UART_TXREG U1TXREG //Tx register



#elif defined (__32MX795F512L__)

#else

#error selected chip not supported

#endif


/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/

/*************************************************************************
 Structure Definitions
 ************************************************************************/


/*************************************************************************
 Enums
 ************************************************************************/

/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/




#endif	/* SYSTEM_H */

