#include "Timer.h"
void (*timer_1_callback) (void);
void (*timer_2_callback) (void);
void (*timer_3_callback) (void);
void (*timer_4_callback) (void);
void (*timer_5_callback) (void);

/*Timer_Setup function*/
//this function initializes a specific timer on the microcontroller with the specified input
//divide is the prescaler for the clock divider
//period is the max value that the timer will count to
//timer is the specific timer that is being worked with
//function_ptr is the ISR function pointer
//enable specifies whether to enable the interrupt or not
void timer_setup(Clock_Divider divide, uint16 period, Timer_Type timer, void *function_ptr, boolean enable) {

    //switch case to determine which timer we are working with
    switch (timer) {
        case Timer_1:
            T1CONbits.TCKPS = divide; //set the clock divider
            PR1 = period; //set the period for the timer
            IPC1bits.T1IP = 7; //set the interrupt to priority level 7
            IEC0bits.T1IE = enable; //enable the interrupt
            timer_1_callback = function_ptr; //set the ISR function pointer
            break;

        case Timer_2:
            T2CONbits.TCKPS = divide; //set the clock divider
            PR2 = period; //set the period for the timer
            IPC2bits.T2IP = 7; //set the interrupt priority
            IEC0bits.T2IE = enable; //set the interrupt enable
            timer_2_callback = function_ptr; //set the ISR function pointer
            break;

        case Timer_3:
            T3CONbits.TCKPS = divide; //set the clock divider
            PR3 = period; //set the period of the timer
            IPC3bits.T3IP = 7; //set the interrupt priotity
            IEC0bits.T3IE = enable; //enable the interrupt
            timer_3_callback = function_ptr; //set the ISR function pointer
        break;

        case Timer_4:
            T4CONbits.TCKPS = divide;
            PR4 = period;
            IPC4bits.T4IP = 7;
            IEC0bits.T4IE = enable;
            timer_4_callback = function_ptr;
            break;

        case Timer_5:
            T5CONbits.TCKPS = divide;
            PR5 = period;
            IPC5bits.T5IP = 7;
            IEC0bits.T5IE = enable;
            timer_5_callback = function_ptr;
            break;
    }
}
/*
void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer_Handler_1(void) {
    INTDisableInterrupts(); //display interrupts


    if (timer_1_callback != NULL) {
        timer_1_callback();
    }

    IFS0bits.T1IF= 0; //clear the interrupt flag
    INTEnableInterrupts(); //reenable interrupts
}


void __ISR(_TIMER_2_VECTOR, IPL7AUTO) Timer_Handler_2(void) {
    INTDisableInterrupts(); //display interrupts


    if (timer_2_callback != NULL) {
        timer_2_callback();
    }

    IFS0bits.T2IF= 0; //clear the interrupt flag
    INTEnableInterrupts(); //reenable interrupts
}

void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer_Handler_3(void) {
    INTDisableInterrupts(); //display interrupts


    if (timer_3_callback != NULL) {
        timer_3_callback();
    }

    IFS0bits.T3IF= 0; //clear the interrupt flag
    INTEnableInterrupts(); //reenable interrupts
}

void __ISR(_TIMER_4_VECTOR, IPL7AUTO) Timer_Handler_4(void) {
    INTDisableInterrupts(); //display interrupts


    if (timer_4_callback != NULL) {
        timer_4_callback();
    }

    IFS0bits.T4IF= 0; //clear the interrupt flag
    INTEnableInterrupts(); //reenable interrupts
}

void __ISR(_TIMER_5_VECTOR, IPL7AUTO) Timer_Handler_5(void) {
    INTDisableInterrupts(); //display interrupts


    if (timer_5_callback != NULL) {
        timer_5_callback();
    }

    IFS0bits.T5IF= 0; //clear the interrupt flag
    INTEnableInterrupts(); //reenable interrupts
}
*/

//This creates ISR's for each of the timers. If additional timers are avaiable on the microcontroller, just add a new element with
//value 6 and above, as relates to the timer name

//if a new microcontroller is used, this function will be need to be changed in teh Timer.h file
Timer_ISR_(1)
Timer_ISR_(2)
Timer_ISR_(3)
Timer_ISR_(4)
Timer_ISR_(5)