/*
 * File:   LEDmain.c
 * Author: Nicholas Yeung
 *
 * Created on 2015-03-07 
 */
//insert configuration for new microcontrollers
#include <xc.h>

//select programming pins
#pragma config ICESEL = ICS_PGx1// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)

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
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 1)

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
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
//programming pins are selected above
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#include <stdio.h>
#include <stdlib.h>
#include "../LED.h"
/*
 * 
 */
void delay(uint wait);
void colorWipe(uint c, uint8 wait) ;
void initialize_pins() {
    RPA2R = 0b0011; //set the RPA2R to SDO1

    TRISAbits.TRISA2=0;//set SDO1 as output
    TRISBbits.TRISB14=0;//set SCK1 as output
   /* PORTAbits.RA0;
    PORTAbits.RA2;
    PORTBbits.RB14;*/
}
int main(int argc, char** argv) {
    uint8 tx_buffer_ptr[200];
    numLEDs=64; //set number of leds that need to light up
    initialize_pins();//initialize pins on microcontroller
    initialize_SPI(295000,15000000,SPI_CH_1,0,0,0,tx_buffer_ptr,200,1,0,0,0);//initialize SPI chennel
    INTEnableSystemMultiVectoredInt();//enable interrupt
    uint8 pixbuf[(numLEDs*3)+1];//size is number of LEDs *3 + 1 latch byte
    pixbuf[(numLEDs+3)]=0; //set the last byte to 0
    pixels=pixbuf;//set pixels point at the pixbuf
    asm volatile ("ei"); //reenable interrupts
    
    /*  Color(127, 127, 127); // White
        Color(127,   0,   0); // Red
        Color(127, 127,   0); // Yellow
        Color(  0, 127,   0); // Green
        Color(  0, 127, 127); // Cyan
        Color(  0,   0, 127); // Blue
        Color(127,   0, 127); // Violet*/
    while(1)
    {
     colorWipe(Color(0,   0,   0), 0);  // Green
    colorWipe(Color(0,   0,   0), 50);  // off
    colorWipe(Color(  0, 127,   0), 0);  // Green
    colorWipe(Color(  0, 127,   0), 50);  // Green
    colorWipe(Color(0,   0,   0), 0);  // off
    colorWipe(Color(0,   0,   0), 50);  // off
    colorWipe(Color(  0, 127,   0), 0);  // Green
    colorWipe(Color(  0, 127,   0), 50);  // Green
    colorWipe(Color(0,   0,   0), 0);  // off
    colorWipe(Color(0,   0,   0), 50);  // off
    colorWipe(Color(  0, 127,   0), 0);  // Green
    colorWipe(Color(  0, 127,   0), 100);  // Green
    colorWipe(Color(0,   0,   0), 0);  // off
    colorWipe(Color(0,   0,   0), 50);  // off
    colorWipe(Color(  0, 127,   0), 0);  // Green
    colorWipe(Color(  0, 127,   0), 100);  // Green
    colorWipe(Color(0,   0,   0), 0);  // off
    colorWipe(Color(0,   0,   0), 50);  // off
    colorWipe(Color(  0, 127,   0), 0);  // Green
    colorWipe(Color(  0, 127,   0), 100);  // Green
    colorWipe(Color(0,   0,   0), 0);  // off
    colorWipe(Color(0,   0,   0), 50);  // off
    delay(10);
    }
    return (EXIT_SUCCESS);
}
void delay(uint wait)
{
    int i=0;
    for (i=0;i<(wait*3000);i++)
    {
    }
}
void colorWipe(uint c, uint8 wait) {
  int i;

  for (i=0; i < numLEDs; i++) {
      setPixelColor(i, c);
      show();
  }
  delay(wait);
}
