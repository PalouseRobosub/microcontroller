/********************************************************
 *   File Name: system.h
 *
 *   Description:
 *              main header file, used for universal constants,
 * configurations for different devices, and transmission protocal defines
 *
 *
 *********************************************************/

#ifndef SYSTEM_H
#define	SYSTEM_H

/*************************************************************************
 System Includes
 ************************************************************************/
#include <peripheral/ports.h>


/*************************************************************************
 Compiler Options
 ************************************************************************/
/* These flags are used to compile for the various different boards.
   Do not activate more than one flag at once!   */

//#define COMPILE_OLD_SUB
//#define COMPILE_SENSOR_BOARD
#define COMPILE_THRUSTER_BOARD
//#define COMPILE_LED_BOARD
//#define COMPILE_ACTUATION_BOARD


/*************************************************************************
 Typedefs
 ************************************************************************/
typedef unsigned int uint;
typedef unsigned short int uint16;
typedef unsigned char uint8;
typedef uint8 boolean;

/*************************************************************************
 Constants
 ************************************************************************/

/*************************************************************************
 Structure Definitions
 ************************************************************************/

/*************************************************************************
 Enums
 ************************************************************************/
enum
{
    FALSE,
    TRUE
};
/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Transmission Protocol Defines
 ************************************************************************/
#ifndef TRANSMISSION_PROTOCOL
#define	TRANSMISSION_PROTOCOL

//CONTROL BYTE (used for packet synchronization)
#define CONTROL_BYTE             '\n'
#define HBRIDGE_ADDRESS1         128
#define HBRIDGE_ADDRESS2         129
#define HBRIDGE_ADDRESS3         130

//////////////////////////
//Incoming (from computer)
//////////////////////////

//Thrusters
#define THRUSTER_BOW_SB 	 0x10
#define THRUSTER_BOW_PORT 	 0x11
#define THRUSTER_DEPTH_SB 	 0x12
#define THRUSTER_DEPTH_PORT 	 0x13
#define THRUSTER_STERN_SB 	 0x14
#define THRUSTER_STERN_PORT 	 0x15

//LED control
#define LED_CONTROL_0     0x30

//////////////////////////
//Outgoing (to computer)
//////////////////////////
#define ACL_0_X     0x10
#define ACL_0_Y     0x11
#define ACL_0_Z     0x12

#define GYRO_0_X    0x20
#define GYRO_0_Y    0x21
#define GYRO_0_Z    0x22

#define ADC_DEPTH   0x30
#define ADC_BATT    0x31

#endif	/* TRANSMISSION_PROTOCOL */



/*************************************************************************
 Board-Specific Defines
 ************************************************************************/
#if defined(COMPILE_OLD_SUB)

//check for other compiler flags
#if defined(COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

#error "multiple compile options were selected"

#endif

//check if the right chip is selected
#if !defined (__32MX795F512L__) 

#error "selected chip is not supported for COMPILE_OLD_SUB option"

#endif

//comm_uart defines
#define COMM_UART UART1
#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_PDSEL U1MODEbits.PDSEL //parity and data selection bits
#define COMM_UART_UTXISEL U1STAbits.UTXISEL //tx interrupt selection bits
#define COMM_UART_UTXEN U1STAbits.UTXEN //tx enable
#define COMM_UART_URXEN U1STAbits.URXEN //rx enable
#define COMM_UART_ON U1MODEbits.ON //uart enable
#define COMM_UART_TX_INT_set(x) IEC0SET = (x << 28) //enables or disables the tx interrupt
#define COMM_UART_TX_INT_clr(x) IEC0CLR = (x << 28) //enables or disables the tx interrupt
#define COMM_UART_RX_INT_set(x) IEC0SET = (x << 27) //enablse or disables the rx interrupt
#define COMM_UART_INT_PRIORITY_set(x) IPC6SET = (x << 2) //sets the priority of the uart interrupts
#define _COMM_UART_VECTOR _UART1_VECTOR //interrupt vector
#define COMM_UART_RXIF IFS0bits.U1RXIF //Rx interrupt flag
#define COMM_UART_TXIF IFS0bits.U1TXIF //Tx interrupt flag
#define COMM_UART_RXREG U1RXREG //Rx register
#define COMM_UART_TXREG U1TXREG //Tx register


//motor_uart defines (MAX32 TX1/RX1 is UART4)
#define MOTOR_UART_BRG U4BRG //baud rate register
#define MOTOR_UART_PDSEL U4MODEbits.PDSEL //parity and data selection bits
#define MOTOR_UART_UTXISEL U4STAbits.UTXISEL //tx interrupt selection bits
#define MOTOR_UART_UTXEN U4STAbits.UTXEN//tx enable
#define MOTOR_UART_URXEN  U4STAbits.URXEN //rx enable
#define MOTOR_UART_ON  U4MODEbits.ON //uart enable
#define MOTOR_UART_TX_INT_set(x) IEC2SET = (x << 5) //enables or disables the tx interrupt
#define MOTOR_UART_TX_INT_clr(x) IEC2CLR = (x << 5) //enables or disables the tx interrupt
#define MOTOR_UART_RX_INT_set(x) IEC2SET = (x << 4) //enables or disables the rx interrupt
#define MOTOR_UART_INT_PRIORITY_set(x) IPC12SET = (x << 10) //sets the priority of the uart interrupts
#define _MOTOR_UART_VECTOR _UART_4_VECTOR //interrupt vector
#define MOTOR_UART_RXIF IFS2bits.U4RXIF //Rx interrupt flag
#define MOTOR_UART_TXIF IFS2bits.U4TXIF //Tx interrupt flag
#define MOTOR_UART_RXREG U4RXREG //Rx register
#define MOTOR_UART_TXREG U4TXREG //Tx register

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

//Sensor Timer defines
#define _SENSOR_TIMER_VECTOR _TIMER_1_VECTOR
#define SENSOR_TIMER_INT_PRIORITY_set(x) IPC1bits.T1IP = x
#define SENSOR_TIMER_INT_set(x) IEC0bits.T1IE = x
#define SENSOR_TIMER_IF IFS0bits.T1IF
#define SENSOR_TIMER_PR PR1
#define SENSOR_TIMER_EN T1CONbits.ON
#define SENSOR_TIMER_PS T1CONbits.TCKPS

//ADC defines
#define ADC_INT_PRIORITY_set(x) IPC6SET = (x << 26) //sets the priority of the ADC interrupt
#define ADC_INT_set(x) IEC1SET = (x << 1) //enables or disables the ADC interrupt
#define ADC_IF IFS1bits.AD1IF //ADC interrupt flag


//LED_SPI defines
#define _LED_SPI_VECTOR _SPI_2_VECTOR
#define LED_SPI_TXIF IFS1bits.SPI2ATXIF
#define LED_SPI_TXIE IEC1bits.SPI2ATXIE
#define LED_SPI_STAT SPI2STAT
#define LED_SPI_BUF  SPI2BUF
#define LED_SPI_BRG  SPI2BRG
#define LED_SPI_TX_INT_PRIORITY_set(x) IPC7SET = (x << 26)
#define LED_SPI_STXISEL SPI2CONbits.STXISEL
#define LED_SPI_MSTEN SPI2CONbits.MSTEN
#define LED_SPI_ON SPI2CONbits.ON
#define LED_SPI_ENHBUF SPI2CONbits.ENHBUF
#define LED_SPI_SPIROV SPI2STATbits.SPIROV
#define LED_SPI_CKP SPI2CONbits.CKP
#define LED_SPI_CKE SPI2CONbits.CKE


#elif defined(COMPILE_SENSOR_BOARD)

//check for other compiler flags
#if defined(COMPILE_OLD_SUB) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

#error "multiple compile options were selected"

#endif

//check if the right chip is selected
#if !defined (__32MX250F128B__)

#error "selected chip is not supported for COMPILE_SENSOR_BOARD option"

#endif

#elif defined(COMPILE_THRUSTER_BOARD)

//check for other compiler flags
#if defined(COMPILE_OLD_SUB) || \
    defined (COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

#error "multiple compile options were selected"

#endif

//check if the right chip is selected
#if !defined (__32MX250F128B__)

#error "selected chip is not supported for COMPILE_THRUSTER_BOARD option"

#endif

//comm_uart defines
#define COMM_UART UART1
#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_PDSEL U1MODEbits.PDSEL //parity and data selection bits
#define COMM_UART_UTXISEL U1STAbits.UTXISEL //tx interrupt selection bits
#define COMM_UART_UTXEN U1STAbits.UTXEN //tx enable
#define COMM_UART_URXEN U1STAbits.URXEN //rx enable
#define COMM_UART_ON U1MODEbits.ON //uart enable
#define COMM_UART_TX_INT_set(x) IEC1SET = (x << 9) //enables or disables the tx interrupt
#define COMM_UART_TX_INT_clr(x) IEC1CLR = (x << 9) //enables or disables the tx interrupt
#define COMM_UART_RX_INT_set(x) IEC1SET = (x << 8) //enablse or disables the rx interrupt
#define COMM_UART_INT_PRIORITY_set(x) IPC8SET = (x << 2) //sets the priority of the uart interrupts
#define _COMM_UART_VECTOR _UART1_VECTOR //interrupt vector
#define COMM_UART_RXIF IFS1bits.U1RXIF //Rx interrupt flag
#define COMM_UART_TXIF IFS1bits.U1TXIF //Tx interrupt flag
#define COMM_UART_RXREG U1RXREG //Rx register
#define COMM_UART_TXREG U1TXREG //Tx register

//Sensor Timer defines
#define _THRUSTER_TIMER_VECTOR _TIMER_1_VECTOR
#define THRUSTER_TIMER_INT_PRIORITY_set(x) IPC1bits.T1IP = x
#define THRUSTER_TIMER_INT_set(x) IEC0bits.T1IE = x
#define THRUSTER_TIMER_IF IFS0bits.T1IF
#define THRUSTER_TIMER_PR PR1
#define THRUSTER_TIMER_EN T1CONbits.ON
#define THRUSTER_TIMER_PS T1CONbits.TCKPS

//LED Pin Mapping
#define BOW_SB_DIR_LED_TRIS     TRISAbits.TRISA0
#define BOW_SB_MAG_LED_TRIS     TRISAbits.TRISA0
#define BOW_PORT_DIR_LED_TRIS   TRISAbits.TRISA0
#define BOW_PORT_MAG_LED_TRIS   TRISAbits.TRISA0
#define STERN_SB_DIR_LED_TRIS   TRISAbits.TRISA0
#define STERN_SB_MAG_LED_TRIS   TRISAbits.TRISA0
#define STERN_PORT_DIR_LED_TRIS TRISAbits.TRISA0
#define STERN_PORT_MAG_LED_TRIS TRISAbits.TRISA0
#define DEPTH_SB_DIR_LED_TRIS   TRISAbits.TRISA0
#define DEPTH_SB_MAG_LED_TRIS   TRISAbits.TRISA0
#define DEPTH_PORT_DIR_LED_TRIS TRISAbits.TRISA0
#define DEPTH_PORT_MAG_LED_TRIS TRISAbits.TRISA0

#define BOW_SB_DIR_LED_PORT     PORTAbits.RA0
#define BOW_SB_MAG_LED_PORT     PORTAbits.RA0
#define BOW_PORT_DIR_LED_PORT   PORTAbits.RA0
#define BOW_PORT_MAG_LED_PORT   PORTAbits.RA0
#define STERN_SB_DIR_LED_PORT   PORTAbits.RA0
#define STERN_SB_MAG_LED_PORT   PORTAbits.RA0
#define STERN_PORT_DIR_LED_PORT PORTAbits.RA0
#define STERN_PORT_MAG_LED_PORT PORTAbits.RA0
#define DEPTH_SB_DIR_LED_PORT   PORTAbits.RA0
#define DEPTH_SB_MAG_LED_PORT   PORTAbits.RA0
#define DEPTH_PORT_DIR_LED_PORT PORTAbits.RA0
#define DEPTH_PORT_MAG_LED_PORT PORTAbits.RA0


#elif defined(COMPILE_LED_BOARD)

//check for other compiler flags
#if defined(COMPILE_OLD_SUB) || \
    defined (COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

#error "multiple compile options were selected"

#endif

//check if the right chip is selected
#if !defined (__32MX250F128B__)

#error "selected chip is not supported for COMPILE_LED_BOARD option"

#endif

#elif defined(COMPILE_ACTUATION_BOARD)

//check for other compiler flags
#if defined(COMPILE_OLD_SUB) || \
    defined (COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD)

#error "multiple compile options were selected"

#endif

//check if the right chip is selected
#if !defined (__32MX250F128B__)

#error "selected chip is not supported for COMPILE_ACTUATION_BOARD option"

#endif

#else //no compile option was selected

#error "no compile option was selected"


#endif


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

