/********************************************************
 *   File Name: ADC_ISR.c
 *
 *   Description:
 *              source code file for the ADC ISR
 *
 *
 *********************************************************/
#include "system.h"
#include "ADC_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/


/********************************************************
 *   Function Name: adc_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void adc_setup(void)
{
    //configure AD1PCFG
    AD1PCFG = 0x00000000;

    //Select the analog inputs to the ADC multiplexers in AD1CHS
    AD1CHSbits.CH0NA = 0; //set the negative input for channel A to internal ground
    AD1CHSbits.CH0SA = 0; //set the positive input to be channel AN0

    TRISAbits.TRISA0 = 1; //make sure the pin is an input (not an output)

    //Select the data format with FORM<2:0> (AD1CON1)
    AD1CON1bits.FORM = 0; //set the format to be simple 16 bit unsigned integer

    //Select the sample clock source using SSRC<2:0> (AD1CON1)
    AD1CON1bits.SSRC = 0b111; //set the clock source to be internal counter

    //Select the voltage reference source using VCFG<2:0> (AD1CON2)
    AD1CON2bits.VCFG = 000; //set the positve and negative references to be Vdd and Vss

    //Select the Scan mode using CSCNA (AD1CON2)
    AD1CON2bits.CSCNA = 0; //do not scan inputs

    //Set the number of conversions per interrupt SMP<3:0> (AD1CON2)
    AD1CON2bits.SMPI = 0; //interrupt after every conversion

    //Set Buffer Fill mode using BUFM (AD1CON2)
    AD1CON2bits.BUFM = 0; //use single 16 word buffer (don't really care, we aren't using the buffer)

    //Select the MUX to be connected to the ADC in ALTS AD1CON2
    AD1CON2bits.ALTS = 0; //always use mux A

    //Select the ADC clock source using ADRC (AD1CON3)
    AD1CON3bits.ADRC = 1; //use internal ADC clock

    //Select the sample time using SAMC<4:0> (AD1CON3)
    AD1CON3bits.SAMC = 15; //sample for 15 ADC clock cycles

    //Select the ADC clock prescaler using ADCS<7:0> (AD1CON3)
    //doesn't matter, we are using the internal oscillator




    //Turn the ADC module on using AD1CON1
    AD1CON1bits.ADON = 1;

    /*
     To configure ADC interrupt (if required):
a) Clear the AD1IF bit (IFS1<1>) (see 17.7).
b) Select ADC interrupt priority AD1IP<2:0> (IPC<28:26>) and subpriority AD1IS<1:0>
(IPC<24:24>) if interrupts are to be used (see 17.7).
15.Start the conversion sequence by initiating sampling (see 17.4.15).
     */

}