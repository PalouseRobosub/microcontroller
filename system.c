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


#if defined(COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)
/********************************************************
 *   Function Name: Configure_PIC32MX250F128B_PPS()
 *
 *   Description: This function configures the peripheral pin select function
 *   for the PIC32MX250F128B microcontroller
 *
 *********************************************************/
void Configure_PIC32MX250F128B_PPS(void) {

    //set all analog pins to be digital
    ANSELA = 0;
    ANSELB = 0;

#if defined (COMPILE_SENSOR_BOARD)
    UART_TX_PPS = 1; //RPB15R
    UART_RX_PPS = 3; //RPB13R
#elif defined(COMPILE_THRUSTER_BOARD)
    //UART_TX_PPS = 2; //RPB10R - UART2
    U1RXR = UART_RX_PPS_PIN; //RPB13R - UART1
#elif defined(COMPILE_LED_BOARD)
    UART_TX_PPS = 1; //RPB4R - U1TX
    U1RXR = UART_RX_PPS_PIN; //RPA2R - U1RX

    SPI_SDO1_PPS = 3; //RPB13R - SDO1
    SPI_SDO2_PPS = 4; //RPA1R - SDO2
#elif defined(COMPILE_ACTUATION_BOARD)
    UART_TX_PPS = 1; //RPB4R
    UART_RX_PPS = 0; //RPA2R
#endif

}
#endif