/********************************************************
 *   File Name: system.c
 *
 *   Description:
 *              main source file, used for defining configuration functions
 *              for different microcontrollers
 *
 *
 *********************************************************/

#include "system.h"

/********************************************************
 *   Function Name: Configure_PIC32MX250F128B_PPS()
 *
 *   Description: This function configures the peripheral pin select function
 *   for the PIC32MX250F128B microcontroller
 *
 *********************************************************/
void Configure_PIC32MX250F128B_PPS(void) {
#if defined (COMPILE_SENSOR_BOARD)
    UART_TX_PPS = 1; //RPB15R
    UART_RX_PPS = 3; //RPB13R
#elif defined(COMPILE_THRUSTER_BOARD)
    UART_TX_PPS = 2; //RPB10R - UART2
    UART_RX_PPS = 3; //RPB13R - UART1
#elif defined(COMPILE_LED_BOARD)
    UART_TX_PPS = 1; //RPB4R
    UART_RX_PPS = 0; //RPA2R

    SPI_SDO1_PPS = 3; //RPB13R
    SPI_SDO2_PPS = 4; //RPA1R
#elif defined(COMPILE_ACTUATION_BOARD)
    UART_TX_PPS = 1; //RPB4R
    UART_RX_PPS = 0; //RPA2R
#endif

}