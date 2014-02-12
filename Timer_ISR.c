/********************************************************
 *   File Name: Timer_ISR.c
 *
 *   Description:
 *              source code file for the Timer ISR
 *
 *
 *********************************************************/
#include "functions.h"
#include "Sensors.h"
#include "Timer_ISR.h"
#include "peripheral/timer.h"
#include "I2C_ISR.h"
#include "comm_UART_ISR.h"
#include "motor_UART_ISR.h"
#include "system.h"
#include "ADC_ISR.h"
#include "LED_SPI_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void timer_1_setup(void)
{
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0xF40);

    IPC1bits.T1IP = 7;
    IEC0bits.T1IE = 1;
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void timer_1_begin(void)
{
    IFS0bits.T1IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer1Handler(void)
{
    extern boolean I2C_BANK_0_is_idle;
    extern boolean COMM_UART_is_idle;
    extern boolean ADC_is_idle;
    extern LED_SPI_QUEUE LED_SPI_Queue;
    extern boolean LED_SPI_is_idle;
    LED_SPI_NODE temp, temp0, temp1;
    int i;
    static boolean flop;
    INTDisableInterrupts();

    //PORTCbits.RC1 = !PORTCbits.RC1; //toggle LED5 (max32)
    i2c_GYRO_Read();
    i2c_ACL_Read();

    
        comm_uart_CreateNode( 'A', 'B', 'C' );
        if (COMM_UART_is_idle)
        {
            comm_uart_begin();
        }
    
    if (I2C_BANK_0_is_idle)
    {
        //PORTAbits.RA3 = !PORTAbits.RA3; //toggle LED4 (max32)
        i2c_bank_0_begin();
    }

    ADC_Depth_Read();
    ADC_Battery_Read();
    if (ADC_is_idle)
    {
        adc_begin();
    }

    temp0.data_G = 0x00;
    temp0.data_R = 0x00;
    temp0.data_B = 0x00;
    led_spi_addToQueue(&LED_SPI_Queue, temp0);



    switch (flop)
    {
        case 1:
            temp.data_G = 0x80;
            temp.data_R = 0x80;
            temp.data_B = 0x81;
            flop = 2;
            break;
        case 2:
            temp.data_G = 0x81;
            temp.data_R = 0x80;
            temp.data_B = 0x80;
            flop = 3;
            break;
        default:
            temp.data_G = 0x80;
            temp.data_R = 0x81;
            temp.data_B = 0x80;
            flop = 1;
    }

//    if (flop)
//    {
//    temp.data_G = 0x80;
//    temp.data_R = 0x81;
//    temp.data_B = 0x80;
//
//    temp1.data_G = 0x80;
//    temp1.data_R = 0x80;
//    temp1.data_B = 0x81;
//    }
//    else
//    {
//        temp1.data_G = 0x80;
//    temp1.data_R = 0x81;
//    temp1.data_B = 0x80;
//
//    temp.data_G = 0x80;
//    temp.data_R = 0x80;
//    temp.data_B = 0x81;
//
//    }
//    flop = !flop;

    for (i=1; i<35; i = i+1)
    {
        led_spi_addToQueue(&LED_SPI_Queue, temp);

    }

    if (LED_SPI_is_idle)
    {
        led_spi_begin();
    }
        
    IFS0bits.T1IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}