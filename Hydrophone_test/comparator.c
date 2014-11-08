/********************************************************
 *   File Name: comparator.c
 *
 *   Description:
 *              source code file for the comparator module
 *
 *
 *********************************************************/


/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "comparator.h"

/*************************************************************************
 Variables
 ************************************************************************/


/********************************************************
 *   Function Name: comparator_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void comparator_setup(void) {

    //Setup Comparator Voltage Reference

    CVRCON = 0; //Initialize everything to 0;

    CVRCONbits.CVRSS = 0;   //Set Comparator Voltage Source to AVDD - AVSS
    CVRCONbits.CVRR = 0;    //Set Range Selection between .25 to .75 Vref
                            //1 Set from 0 to .67
    CVRCONbits.CVROE = 1;    //Voltage level is output to CVrefout pin

    //Setup Comparator
    CM1CON = 0; //Initialize Control Registers to 0
    CM2CON = 0;
    CM3CON = 0;

    //Inverting input is connected CxINB pin 01 CxINC pin 10 CxIND pin 11 IVref
    CM1CONbits.CCH = 0b00;
    CM2CONbits.CCH = 0b00;
    CM3CONbits.CCH = 0b00;

   //non-inverting input connected to internal CVref
    CM1CONbits.CREF = 1;
    CM2CONbits.CREF = 1;
    CM3CONbits.CREF = 1;

    //Interrupt Generation
    //Output is high when input voltage is above CVref with CPOL = 1;
    //00 no interrupt generated
    //01 low-to-high
    //10 high-to-low
    //11 low-to-high and high-to-low
    CM1CONbits.EVPOL = 0b01;
    CM2CONbits.EVPOL = 0b01;
    CM3CONbits.EVPOL = 0b01;

    //Output is 1 when inverting pin is higher than non-inverting pin
    CM1CONbits.CPOL = 1;
    CM2CONbits.CPOL = 1;
    CM3CONbits.CPOL = 1;

    //Enable interrupts and clear flags
    IPC6bits.CMP1IP = 7;
    IPC7bits.CMP2IP = 7;
    IPC7bits.CMP3IP = 7;

    IFS1bits.CMP1IF = 0;
    IFS1bits.CMP2IF = 0;
    IFS1bits.CMP3IF = 0;

    IEC1bits.CMP1IE = 1;
    IEC1bits.CMP2IE = 1;
    IEC1bits.CMP3IE = 1;

    //Setting up pins as inputs
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB14 = 1;

    //Voltage Reference Setting
    //CVref = 1/4*CVrsrc + CVR/32 * CVrsrc
    CVRCONbits.CVR = 0x4;

    //Turn on Comparators and Comparator Voltage Reference
    CVRCONbits.ON = 1;
    CM1CONbits.ON = 1;
    CM2CONbits.ON = 1;
    CM3CONbits.ON = 1;

}



/********************************************************
 *   Function Name: comparator_handler()
 *
 *   Description:
 *
 *
 *********************************************************/
//you can have all three interrupts map to the same vector
//void __ISR((_COMPARATOR_1_VECTOR, _COMPARATOR_2_VECTOR, _COMPARATOR_3_VECTOR), IPL7AUTO) comparator_handler(void) {
//or you could create individual ISRs for each interrupt
void __ISR(_COMPARATOR_1_VECTOR, IPL7AUTO) comparator_handler(void) {

    extern int time_stamp [3],
        comp_trig [3],
        instance, error;
    
    INTDisableInterrupts();

    T1CONbits.ON = 1;

    time_stamp[instance] = TMR1;
    comp_trig[instance] = IFS1;
       
    
    instance++;

    IFS1CLR = 0b0111; //clear interrupt flag bits

    INTEnableInterrupts();

}

void __ISR(_COMPARATOR_2_VECTOR, IPL7AUTO) comparator_handler(void);
void __ISR(_COMPARATOR_3_VECTOR, IPL7AUTO) comparator_handler(void);
