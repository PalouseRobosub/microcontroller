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
 Sensor_motor Defines
 ************************************************************************/
#ifndef SENSORS_MOTORS
#define	SENSORS_MOTORS

//CONTROLL BYTE
#define CONTROL_BYTE             '\n'

//motors
#define THRUSTER_BOW_SB 	 0x10
#define THRUSTER_BOW_PORT 	 0x11
#define THRUSTER_DEPTH_SB 	 0x12
#define THRUSTER_DEPTH_PORT 	 0x13
#define THRUSTER_STERN_SB 	 0x14
#define THRUSTER_STERN_PORT 	 0x15

//sensors
#define ACCELEROMETER            0x21
#define GYROSCOPE                0x22

#endif	/* SENSORS_MOTORS */
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

//motor_uart defines
#define MOTOR_UART UART2
#define MOTOR_UART_BRG U2BRG //baud rate register
#define MOTOR_UART_PDSEL U2MODEbits.PDSEL //parity and data selection bits
#define MOTOR_UART_UTXISEL U2STAbits.UTXISEL //tx interrupt selection bits
#define MOTOR_UART_UTXEN U2STAbits.UTXEN//tx enable
#define MOTOR_UART_URXEN  U2STAbits.URXEN //rx enable
#define MOTOR_UART_ON  U2MODEbits.ON //uart enable
#define MOTOR_UART_TX_INT_set(x) IEC1SET = (x << 10) //enables or disables the tx interrupt
#define MOTOR_UART_RX_INT_set(x) IEC1SET = (x << 9) //enables or disables the rx interrupt
#define MOTOR_UART_INT_PRIORITY_set(x) IPC8SET = (x << 2) //sets the priority of the uart interrupts
#define _MOTOR_UART_VECTOR _UART2_VECTOR //interrupt vector
#define MOTOR_UART_RXIF IFS1bits.U2RXIF //Rx interrupt flag
#define MOTOR_UART_TXIF IFS1bits.U2TXIF //Tx interrupt flag
#define MOTOR_UART_RXREG U2RXREG //Rx register
#define MOTOR_UART_TXREG U2TXREG //Tx register

//I2C Bank 0 defines
#define _I2C_BANK_0_VECTOR _I2C_1_VECTOR
#define I2C_BANK_0_BRG I2C1BRG //baud rate register
#define I2C_BANK_0_INT_PRIORITY_set(x) IPC6SET = (x << 10)
#define I2C_BANK_0_INT_set(x) IEC0SET = (x << 31)
#define I2C_BANK_0_ON I2C1CONbits.ON
#define I2C_BANK_0_MIF IFS0bits.I2C1MIF
#define I2C_BANK_0_TRN I2C1TRN
#define I2C_BANK_0_RSEN I2C1CONbits.RSEN
#define I2C_BANK_0_PEN I2C1CONbits.PEN
#define I2C_BANK_0_RCEN I2C1CONbits.RCEN
#define I2C_BANK_0_RCV I2C1RCV
#define I2C_BANK_0_ACKDT I2C1CONbits.ACKDT
#define I2C_BANK_0_ACKEN I2C1CONbits.ACKEN
#define I2C_BANK_0_SEN I2C1CONbits.SEN






#elif defined (__32MX795F512L__)

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


//motor_uart defines
#define MOTOR_UART UART2
#define MOTOR_UART_BRG U2BRG //baud rate register
#define MOTOR_UART_PDSEL U2MODEbits.PDSEL //parity and data selection bits
#define MOTOR_UART_UTXISEL U2STAbits.UTXISEL //tx interrupt selection bits
#define MOTOR_UART_UTXEN U2STAbits.UTXEN//tx enable
#define MOTOR_UART_URXEN  U2STAbits.URXEN //rx enable
#define MOTOR_UART_ON  U2MODEbits.ON //uart enable
#define MOTOR_UART_TX_INT_set(x) IEC1SET = (x << 10) //enables or disables the tx interrupt
#define MOTOR_UART_RX_INT_set(x) IEC1SET = (x << 9) //enables or disables the rx interrupt
#define MOTOR_UART_INT_PRIORITY_set(x) IPC8SET = (x << 2) //sets the priority of the uart interrupts
#define _MOTOR_UART_VECTOR _UART2_VECTOR //interrupt vector
#define MOTOR_UART_RXIF IFS1bits.U2RXIF //Rx interrupt flag
#define MOTOR_UART_TXIF IFS1bits.U2TXIF //Tx interrupt flag
#define MOTOR_UART_RXREG U2RXREG //Rx register
#define MOTOR_UART_TXREG U2TXREG //Tx register

//I2C Bank 0 defines
#define _I2C_BANK_0_VECTOR _I2C_1_VECTOR
#define I2C_BANK_0_BRG I2C1BRG //baud rate register
#define I2C_BANK_0_INT_PRIORITY_set(x) IPC6SET = (x << 10)
#define I2C_BANK_0_INT_set(x) IEC0SET = (x << 31)
#define I2C_BANK_0_ON I2C1CONbits.ON
#define I2C_BANK_0_MIF IFS0bits.I2C1MIF
#define I2C_BANK_0_TRN I2C1TRN
#define I2C_BANK_0_RSEN I2C1CONbits.RSEN
#define I2C_BANK_0_PEN I2C1CONbits.PEN
#define I2C_BANK_0_RCEN I2C1CONbits.RCEN
#define I2C_BANK_0_RCV I2C1RCV
#define I2C_BANK_0_ACKDT I2C1CONbits.ACKDT
#define I2C_BANK_0_ACKEN I2C1CONbits.ACKEN
#define I2C_BANK_0_SEN I2C1CONbits.SEN



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

