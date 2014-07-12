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
/*This define suppresses warning that new versions of compiler will not support
 * periopheral/ports.h. New teams might need to download legacy compiler in the future
 */
#define _SUPPRESS_PLIB_WARNING
#include <peripheral/ports.h>


/*************************************************************************
 Compiler Options
 ************************************************************************/
/* These flags are used to compile for the various different boards.
   Do not activate more than one flag at once!   */

//#define COMPILE_OLD_SUB
//#define COMPILE_SENSOR_BOARD
//#define COMPILE_THRUSTER_BOARD
//#define COMPILE_LED_BOARD
#define COMPILE_ACTUATION_BOARD


/*************************************************************************
 Typedefs
 ************************************************************************/
typedef unsigned int uint;
typedef unsigned short int uint16;
typedef unsigned char uint8;
typedef signed char sint8;
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
enum {
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

//H-BRIDGE ADDRESSES
#define HBRIDGE_ADDRESS1         128
#define HBRIDGE_ADDRESS2         129
#define HBRIDGE_ADDRESS3         130

//////////////////////////
//Incoming (from computer)
//////////////////////////

//Special Commands
#define I2C_0_RESET_CMD          0x01
#define I2C_1_RESET_CMD          0x02


//Thrusters
#define THRUSTER_BOW_SB 	 0x10
#define THRUSTER_BOW_PORT 	 0x11
#define THRUSTER_DEPTH_SB 	 0x12
#define THRUSTER_DEPTH_PORT 	 0x13
#define THRUSTER_STERN_SB 	 0x14
#define THRUSTER_STERN_PORT 	 0x15

//Actuation
#define PNEUMATIC_TORPEDO_R     0x20
#define PNEUMATIC_TORPEDO_L     0x21
#define PNEUMATIC_MARKER_R      0x22
#define PNEUMATIC_MARKER_L      0x23
#define PNEUMATIC_CLAW_OPEN     0x24
#define PNEUMATIC_CLAW_CLOSE    0x25

#define STEPPER_FRONT            0x26
#define STEPPER_BOTTOM           0x27

//LED control
#define LED_CONTROL_0           0x30
#define LED_CONTROL_1           0x31

//////////////////////////
//Outgoing (to computer)
//////////////////////////
#define ACL_0_X     0x10
#define ACL_0_Y     0x11
#define ACL_0_Z     0x12

#define ACL_1_X     0x13
#define ACL_1_Y     0x14
#define ACL_1_Z     0x15

#define GYRO_0_X    0x20
#define GYRO_0_Y    0x21
#define GYRO_0_Z    0x22

#define ADC_DEPTH   0x30
#define ADC_BATT    0x31

#define MAG_0_X    0x40
#define MAG_0_Y    0x41
#define MAG_0_Z    0x42

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
#define COMM_UART_BAUD_RATE_DIV 10
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

//Sensor Timer Configurations
#define SENSOR_TIMER_PRESCALER		0b11
#define SENSOR_TIMER_PERIOD_REGISTER	0xF40

//ADC defines
#define ADC_INT_PRIORITY_set(x) IPC6SET = (x << 26) //sets the priority of the ADC interrupt
#define ADC_INT_set(x) IEC1SET = (x << 1) //enables or disables the ADC interrupt
#define ADC_IF IFS1bits.AD1IF //ADC interrupt flag
#define ADC_BATTERY_INPUT_PIN TRISAbits.TRISA6
#define ADC_DEPTH_INPUT_PIN TRISAbits.TRISA7

//LED_SPI defines
#define _LED_SPI_0_VECTOR _SPI_2_VECTOR
#define LED_SPI_0_TXIF IFS1bits.SPI2ATXIF
#define LED_SPI_0_TXIE IEC1bits.SPI2ATXIE
#define LED_SPI_0_STAT SPI2STAT
#define LED_SPI_0_BUF  SPI2BUF
#define LED_SPI_0_BRG  SPI2BRG
#define LED_SPI_0_CLK_DIV 0x04
#define LED_SPI_0_TX_INT_PRIORITY_set(x) IPC7SET = (x << 26)
#define LED_SPI_0_STXISEL SPI2CONbits.STXISEL
#define LED_SPI_0_MSTEN SPI2CONbits.MSTEN
#define LED_SPI_0_ON SPI2CONbits.ON
#define LED_SPI_0_ENHBUF SPI2CONbits.ENHBUF
#define LED_SPI_0_SPIROV SPI2STATbits.SPIROV
#define LED_SPI_0_CKP SPI2CONbits.CKP
#define LED_SPI_0_CKE SPI2CONbits.CKE

//Sensor defines
#define ACL_0_device_address		0x1D
#define ACL_0_WRITE_sub_address		0x2D
#define ACL_0_WRITE_address_size		1
#define ACL_0_WRITE_data_size		1
#define ACL_0_WRITE_tx_data		0x08
#define ACL_0_READ_sub_address		0x32
#define ACL_0_READ_address_size		1
#define ACL_0_READ_data_size		6
#define ACL_0_READ_tx_data		0x08

#define GYRO_0_device_address		0x69
#define GYRO_0_WRITE_sub_address		0x20
#define GYRO_0_WRITE_address_size	1
#define GYRO_0_WRITE_data_size		1
#define GYRO_0_WRITE_tx_data		0x0F
#define GYRO_0_READ_sub_address		0x28
#define GYRO_0_READ_address_size		1
#define GYRO_0_READ_data_size		6

#define MAG_0_device_address		0x0E
#define MAG_0_CTRL_REG                  0x10
#define MAG_0_CTRL_REG_data		0x01
#define MAG_0_DATA_sub_address		0x01
#define MAG_0_READ_data_size		6

#define ADC_DEPTH_CH    ADC_CH_7
#define ADC_BATTERY_CH  ADC_CH_6

//pneumatics
// Solenoid 1 G6
// Solenoid 2 G9
// Solenoid 3 D11
// Solenoid 4 G8
// Solenoid 5 B13
// Solenoid 6 E6
#define PNEUMATIC_TORPEDO_R_TRIS    TRISGbits.TRISG6
#define PNEUMATIC_TORPEDO_L_TRIS    TRISGbits.TRISG9
#define PNEUMATIC_MARKER_R_TRIS     TRISDbits.TRISD11
#define PNEUMATIC_MARKER_L_TRIS     TRISGbits.TRISG8
#define PNEUMATIC_CLAW_OPEN_TRIS    TRISBbits.TRISB13
#define PNEUMATIC_CLAW_CLOSE_TRIS   TRISEbits.TRISE6

#define PNEUMATIC_TORPEDO_R_PIN     PORTGbits.RG6
#define PNEUMATIC_TORPEDO_L_PIN     PORTGbits.RG9
#define PNEUMATIC_MARKER_R_PIN      PORTDbits.RD11
#define PNEUMATIC_MARKER_L_PIN      PORTGbits.RG8
#define PNEUMATIC_CLAW_OPEN_PIN     PORTBbits.RB13
#define PNEUMATIC_CLAW_CLOSE_PIN    PORTEbits.RE6


//END OLD_SUB


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

//comm_uart defines
#define COMM_UART UART1
#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_BAUD_RATE_DIV 16
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
#define _SENSOR_TIMER_VECTOR _TIMER_1_VECTOR
#define SENSOR_TIMER_INT_PRIORITY_set(x) IPC1bits.T1IP = x
#define SENSOR_TIMER_INT_set(x) IEC0bits.T1IE = x
#define SENSOR_TIMER_IF IFS0bits.T1IF
#define SENSOR_TIMER_PR PR1
#define SENSOR_TIMER_EN T1CONbits.ON
#define SENSOR_TIMER_PS T1CONbits.TCKPS

//Sensor Timer Configurations
#define SENSOR_TIMER_PRESCALER		0b11
#define SENSOR_TIMER_PERIOD_REGISTER	5859

//I2C Bank 0 defines
#define _I2C_BANK_0_VECTOR _I2C_1_VECTOR
#define I2C_BANK_0_BRG I2C1BRG //baud rate register
#define I2C_BANK_0_CLK_DIV 74
#define I2C_BANK_0_INT_PRIORITY_set(x) IPC8SET = (x << 10)
#define I2C_BANK_0_INT_set(x) IEC1SET = (x << 12)
#define I2C_BANK_0_ON I2C1CONbits.ON
#define I2C_BANK_0_MIF IFS1bits.I2C1MIF
#define I2C_BANK_0_TRN I2C1TRN
#define I2C_BANK_0_RSEN I2C1CONbits.RSEN
#define I2C_BANK_0_PEN I2C1CONbits.PEN
#define I2C_BANK_0_RCEN I2C1CONbits.RCEN
#define I2C_BANK_0_RCV I2C1RCV
#define I2C_BANK_0_ACKDT I2C1CONbits.ACKDT
#define I2C_BANK_0_ACKEN I2C1CONbits.ACKEN
#define I2C_BANK_0_SEN I2C1CONbits.SEN

#define I2C_BANK_0_RESET_TRIS TRISBbits.TRISB7
#define I2C_BANK_0_RESET_PIN  LATBbits.LATB7

//ADC defines
#define ADC_INT_PRIORITY_set(x) IPC5SET = (x << 26) //sets the priority of the ADC interrupt
#define ADC_INT_set(x) IEC0SET = (x << 28) //enables or disables the ADC interrupt
#define ADC_IF IFS0bits.AD1IF //ADC interrupt flag
#define ADC_BATTERY_INPUT_PIN TRISAbits.TRISA0
#define ADC_DEPTH_INPUT_PIN TRISAbits.TRISA1

//Sensor defines
#define ACL_0_device_address		0x1D
#define ACL_0_WRITE_sub_address		0x2D
#define ACL_0_WRITE_address_size		1
#define ACL_0_WRITE_data_size		1
#define ACL_0_WRITE_tx_data		0x08
#define ACL_0_READ_sub_address		0x32
#define ACL_0_READ_address_size		1
#define ACL_0_READ_data_size		6
#define ACL_0_READ_tx_data		0x08

#define GYRO_0_device_address		0x69
#define GYRO_0_WRITE_sub_address		0x20
#define GYRO_0_WRITE_address_size	1
#define GYRO_0_WRITE_data_size		1
#define GYRO_0_WRITE_tx_data		0x0F
#define GYRO_0_READ_sub_address		29
#define GYRO_0_READ_address_size		1
#define GYRO_0_READ_data_size		6

#define MAG_0_device_address		0x0E
#define MAG_0_CTRL_REG                  0x10
#define MAG_0_CTRL_REG_data		0x01
#define MAG_0_DATA_sub_address		0x01
#define MAG_0_READ_data_size		6

#define ADC_DEPTH_CH    ADC_CH_1
#define ADC_BATTERY_CH  ADC_CH_0

//PPS Defines
#define UART_TX_PPS     RPB15R
#define UART_RX_PPS     3 //RPB13R


//END SENSOR_BOARD

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
#define COMM_UART_BAUD_RATE_DIV 194
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

//Thruster Timer defines
#define _THRUSTER_TIMER_VECTOR _TIMER_1_VECTOR
#define THRUSTER_TIMER_INT_PRIORITY_set(x) IPC1bits.T1IP = x
#define THRUSTER_TIMER_INT_set(x) IEC0bits.T1IE = x
#define THRUSTER_TIMER_IF IFS0bits.T1IF
#define THRUSTER_TIMER_PR PR1
#define THRUSTER_TIMER_EN T1CONbits.ON
#define THRUSTER_TIMER_PS T1CONbits.TCKPS

//Actuation Timer Configurations
#define THRUSTER_TIMER_PRESCALER		0b01
#define THRUSTER_TIMER_PERIOD_REGISTER         300

//LED Pin Mapping
#define BOW_SB_DIR_LED_TRIS     TRISAbits.TRISA3  //DIR 1
#define BOW_SB_MAG_LED_TRIS     TRISBbits.TRISB15 //SPD 1
#define BOW_PORT_DIR_LED_TRIS   TRISAbits.TRISA2  //DIR 2
#define BOW_PORT_MAG_LED_TRIS   TRISBbits.TRISB14 //SPD 2
#define STERN_SB_DIR_LED_TRIS   TRISBbits.TRISB3  //DIR 3
#define STERN_SB_MAG_LED_TRIS   TRISBbits.TRISB11 //SPD 3
#define STERN_PORT_DIR_LED_TRIS TRISBbits.TRISB2  //DIR 4
#define STERN_PORT_MAG_LED_TRIS TRISBbits.TRISB4  //SPD 4
#define DEPTH_SB_DIR_LED_TRIS   TRISBbits.TRISB1  //DIR 5
#define DEPTH_SB_MAG_LED_TRIS   TRISAbits.TRISA4  //SPD 5
#define DEPTH_PORT_DIR_LED_TRIS TRISBbits.TRISB0  //DIR 6
#define DEPTH_PORT_MAG_LED_TRIS TRISBbits.TRISB5  //SPD 6

#define BOW_SB_DIR_LED_PORT     LATAbits.LATA3  //DIR 1
#define BOW_SB_MAG_LED_PORT     LATBbits.LATB15 //SPD 1
#define BOW_PORT_DIR_LED_PORT   LATAbits.LATA2  //DIR 2
#define BOW_PORT_MAG_LED_PORT   LATBbits.LATB14 //SPD 2
#define STERN_SB_DIR_LED_PORT   LATBbits.LATB3  //DIR 3
#define STERN_SB_MAG_LED_PORT   LATBbits.LATB11 //SPD 3
#define STERN_PORT_DIR_LED_PORT LATBbits.LATB2  //DIR 4
#define STERN_PORT_MAG_LED_PORT LATBbits.LATB4  //SPD 4
#define DEPTH_SB_DIR_LED_PORT   LATBbits.LATB1  //DIR 5
#define DEPTH_SB_MAG_LED_PORT   LATAbits.LATA4  //SPD 5
#define DEPTH_PORT_DIR_LED_PORT LATBbits.LATB0  //DIR 6
#define DEPTH_PORT_MAG_LED_PORT LATBbits.LATB5  //SPD 6

//PPS Defines
#define UART_TX_PPS     RPB3R
#define UART_RX_PPS_PIN     3 //RPB13R


//END THRUSTER_BOARD

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

//comm_uart defines
#define COMM_UART UART1
#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_BAUD_RATE_DIV 16
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

//LED_SPI defines
#define _LED_SPI_0_VECTOR _SPI_1_VECTOR
#define LED_SPI_0_TXIF IFS1bits.SPI1TXIF
#define LED_SPI_0_TXIE IEC1bits.SPI1TXIE
#define LED_SPI_0_STAT SPI1STAT
#define LED_SPI_0_BUF  SPI1BUF
#define LED_SPI_0_BRG  SPI1BRG
#define LED_SPI_0_CLK_DIV 0x0C
#define LED_SPI_0_TX_INT_PRIORITY_set(x) IPC7SET = (x << 26)
#define LED_SPI_0_STXISEL SPI1CONbits.STXISEL
#define LED_SPI_0_MSTEN SPI1CONbits.MSTEN
#define LED_SPI_0_ON SPI1CONbits.ON
#define LED_SPI_0_ENHBUF SPI1CONbits.ENHBUF
#define LED_SPI_0_SPIROV SPI1STATbits.SPIROV
#define LED_SPI_0_CKP SPI1CONbits.CKP
#define LED_SPI_0_CKE SPI1CONbits.CKE

#define _LED_SPI_1_VECTOR _SPI_2_VECTOR
#define LED_SPI_1_TXIF IFS1bits.SPI2TXIF
#define LED_SPI_1_TXIE IEC1bits.SPI2TXIE
#define LED_SPI_1_STAT SPI2STAT
#define LED_SPI_1_BUF  SPI2BUF
#define LED_SPI_1_BRG  SPI2BRG
#define LED_SPI_1_CLK_DIV 120
#define LED_SPI_1_TX_INT_PRIORITY_set(x) IPC9SET = (x << 2)
#define LED_SPI_1_STXISEL SPI2CONbits.STXISEL
#define LED_SPI_1_MSTEN SPI2CONbits.MSTEN
#define LED_SPI_1_ON SPI2CONbits.ON
#define LED_SPI_1_ENHBUF SPI2CONbits.ENHBUF
#define LED_SPI_1_SPIROV SPI2STATbits.SPIROV
#define LED_SPI_1_CKP SPI2CONbits.CKP
#define LED_SPI_1_CKE SPI2CONbits.CKE

//PPS Defines
#define UART_TX_PPS     RPB4R
#define UART_RX_PPS_PIN     RPA2R
#define SPI_SDO1_PPS     RPB13R
#define SPI_SDO2_PPS     RPA1R


//END LED_BOARD

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

//comm_uart defines
#define COMM_UART UART1
#define COMM_UART_BRG U1BRG //baud rate register
#define COMM_UART_BAUD_RATE_DIV 16
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

//Pneumatic Defines
#define PNEUMATIC_TORPEDO_R_TRIS    TRISBbits.TRISB3
#define PNEUMATIC_TORPEDO_L_TRIS    TRISBbits.TRISB2

#define PNEUMATIC_TORPEDO_R_PIN    LATBbits.LATB3
#define PNEUMATIC_TORPEDO_L_PIN    LATBbits.LATB2

#define TORPEDO_DURATION 2

//PPS Defines
#define UART_TX_PPS     RPB4R
#define UART_RX_PPS     RPA2R




#define CW 			0
#define CCW			1
#define FULLSTEP		0
#define HALFSTEP		1
#define BTN1			BIT_6
#define BTN2			BIT_7
#define STEPPER_MASK  	(BIT_7 | BIT_8 | BIT_9 | BIT_10)
#define LEDA			BIT_2
#define LEDB			BIT_3
#define LEDC			BIT_4
#define TCKS_PER_MS		1250
#define PRESCALE_8		0x8010
#define NUM_MS			1426

#define RESET_COMMAND           0xF0
#define DROP1_COMMAND           0xF1
#define DROP2_COMMAND           0xF2
#define CLOSE_COMMAND           0xF3
#define STOP_COMMAND            0xF4
#define STEPPER_EN_ON           0xF5
#define STEPPER_EN_OFF          0xF6

#define FRONT_STEPPER           0
#define BOTTOM_STEPPER          1
#define DIR_OPEN                0
#define DIR_CLOSED              1

#define EN_ON                   1
#define EN_OFF                  0
#define STEPS_PER_POS           10
#define STEP_PERIOD             50

#define DROP1_POS               40
#define DROP2_POS               60

#define MAX_POS                 300


enum STEPPER_STATE {
    S_0_5, S_1_0, S_1_5, S_2_0, S_2_5, S_3_0, S_3_5, S_4_0
};

//Sensor Timer defines
#define _ACTUATION_TIMER_VECTOR _TIMER_1_VECTOR
#define ACTUATION_TIMER_INT_PRIORITY_set(x) IPC1bits.T1IP = x
#define ACTUATION_TIMER_INT_set(x) IEC0bits.T1IE = x
#define ACTUATION_TIMER_IF IFS0bits.T1IF
#define ACTUATION_TIMER_PR PR1
#define ACTUATION_TIMER_EN T1CONbits.ON
#define ACTUATION_TIMER_PS T1CONbits.TCKPS

//Actuation Timer Configurations
#define ACTUATION_TIMER_PRESCALER		0b11
#define ACTUATION_TIMER_PERIOD_REGISTER         0x140  //0x5Fff//0xF40

//GPIO Pins
#define SEN1_TRIS        TRISAbits.TRISA0  //SEN1 RA0 pin 2
#define SEN2_TRIS        TRISAbits.TRISA1  //SEN2 RA1 pin 3
#define STEP_IN11_TRIS   TRISBbits.TRISB11 //STEP_IN11 RB11 pin 22
#define STEP_IN12_TRIS   TRISBbits.TRISB13 //STEP_IN12 RB13 pin 24
#define STEP_IN13_TRIS   TRISBbits.TRISB15 //STEP_IN13 RB15 pin 26
#define STEP_IN14_TRIS   TRISBbits.TRISB14 //STEP_IN14 RB14 pin 25
#define STEP_IN21_TRIS   TRISBbits.TRISB7  //STEP_IN21 RB7 pin 16
#define STEP_IN22_TRIS   TRISBbits.TRISB5  //STEP_IN22 RB5 pin 14
#define STEP_IN23_TRIS   TRISAbits.TRISA3  //STEP_IN23 RA3 pin 10
#define STEP_IN24_TRIS   TRISBbits.TRISB10 //STEP_IN24 RB10 pin 21
#define STEP_EN_TRIS     TRISAbits.TRISA4  //STEP_EN RA4 pin 12

#define SEN1_PIN        PORTAbits.RA1  //SEN1 RA0 pin 2
#define SEN2_PIN        PORTAbits.RA0  //SEN2 RA1 pin 3
#define STEP_IN11_PIN   LATBbits.LATB11 //STEP_IN11 RB11 pin 22
#define STEP_IN12_PIN   LATBbits.LATB13 //STEP_IN12 RB13 pin 24
#define STEP_IN13_PIN   LATBbits.LATB15 //STEP_IN13 RB15 pin 26
#define STEP_IN14_PIN   LATBbits.LATB14 //STEP_IN14 RB14 pin 25
#define STEP_IN21_PIN   LATBbits.LATB7  //STEP_IN21 RB7 pin 16
#define STEP_IN22_PIN   LATBbits.LATB5  //STEP_IN22 RB5 pin 14
#define STEP_IN23_PIN   LATAbits.LATA3  //STEP_IN23 RA3 pin 10
#define STEP_IN24_PIN   LATBbits.LATB10 //STEP_IN24 RB10 pin 21
#define STEP_EN_PIN     LATAbits.LATA4  //STEP_EN RA4 pin 12

//END ACTUATION_BOARD

#else //no compile option was selected

#error "no compile option was selected"


#endif


/*************************************************************************
 Function Declarations
 ************************************************************************/

/********************************************************
 *   Function Name: Configure_PIC32MX250F128B_PPS()
 *
 *   Description: This function configures the peripheral pin select function
 *   for the PIC32MX250F128B microcontroller
 *
 *********************************************************/
void Configure_PIC32MX250F128B_PPS(void);



#endif	/* SYSTEM_H */

