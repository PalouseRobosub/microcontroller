/********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#if defined (COMPILE_OLD_SUB)

#include "Sensors.h"
#include "I2C_ISR.h"
#include "SENSOR_TIMER_ISR.h"
#include "comm_UART_ISR.h"
#include "motor_UART_ISR.h"
#include "ADC_ISR.h"
#include "LED_SPI_ISR.h"

#elif defined (COMPILE_SENSOR_BOARD)
#include "Sensors.h"
#include "I2C_ISR.h"
#include "SENSOR_TIMER_ISR.h"
#include "comm_UART_ISR.h"
#include "ADC_ISR.h"
#elif defined (COMPILE_THRUSTER_BOARD)
#include "SENSOR_TIMER_ISR.h"
#include "comm_UART_ISR.h"
#elif defined (COMPILE_LED_BOARD)
#include "comm_UART_ISR.h"
#include "LED_SPI_ISR.h"
#elif defined (COMPILE_ACTUATION_BOARD)
#include "comm_UART_ISR.h"
#include "ACTUATION_TIMER_ISR.h"
#include "GPIO.h"
#endif


/*************************************************************************
 Processor Configuration
 ************************************************************************/
#if defined(COMPILE_OLD_SUB)
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#pragma config ICESEL = ICS_PGx2

#elif defined(COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

//insert configuration for new microcontrollers
#include <xc.h>

//select programming pins
#if defined (COMPILE_SENSOR_BOARD) || defined (COMPILE_LED_BOARD)
#pragma config ICESEL = ICS_PGx1// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#elif defined (COMPILE_THRUSTER_BOARD)
#pragma config ICESEL = ICS_PGx3// ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#endif

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
//programming pins are selected above
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)
#endif

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

#if defined (COMPILE_OLD_SUB)
    //setup/configure hardware modules
    sensor_timer_setup();
    i2c_bank_0_setup();
    comm_uart_setup();
    motor_uart_setup();
    adc_setup();
    led_spi_0_setup();
    GPIO_setup();

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();
    i2c_GYRO_Initialize();
    i2c_MAG_Initialize();

    //start each ISR
    i2c_bank_0_begin();
    motor_uart_begin();
    comm_uart_begin();
    led_spi_0_begin();

#elif defined (COMPILE_SENSOR_BOARD)
    //setup/configure hardware modules
    Configure_PIC32MX250F128B_PPS();
    sensor_timer_setup();
    i2c_bank_0_setup();
    comm_uart_setup();
    adc_setup();

    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();
    i2c_GYRO_Initialize();
    i2c_MAG_Initialize();

    //start each ISR
    i2c_bank_0_begin();
    comm_uart_begin();

#elif defined (COMPILE_THRUSTER_BOARD)
    //setup/configure hardware modules
    Configure_PIC32MX250F128B_PPS();
    thruster_timer_setup();
    comm_uart_setup();
    thruster_LED_setup();

    //load nodes onto queues to initialize sensors

    //start each ISR
    comm_uart_begin();


#elif defined (COMPILE_LED_BOARD)
    //setup/configure hardware modules
    Configure_PIC32MX250F128B_PPS();
    comm_uart_setup();
    led_spi_0_setup();
    led_spi_1_setup();

    //load nodes onto queues to initialize sensors
    //initialize LEDS maybe? Either off or a particular pattern

    //start each ISR
    comm_uart_begin();
    led_spi_0_begin();
    led_spi_1_begin();

#elif defined (COMPILE_ACTUATION_BOARD)
    //setup/configure hardware modules
    Configure_PIC32MX250F128B_PPS();
    comm_uart_setup();
    actuation_timer_setup();
    stepper_motor_setup();	// Initializes system
    GPIO_setup();

    //possibly start with reseting both motors?
    //this could be an issue if the microcontroller randomly resets
    //during runtime, especially if we currently holding something.
    //Or the computer could do it.


#endif

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    while (1) {
        //background tasks

#if defined (COMPILE_OLD_SUB) || \
    defined (COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)
        bg_process_comm_uart();


#elif defined (COMPILE_THRUSTER_BOARD)
        bg_process_thruster_comm_uart();

#endif


    }

    return 0;
}

