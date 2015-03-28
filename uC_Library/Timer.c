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

void initialize_TIMER(Timer_Config config) {

    //switch case to determine which timer we are working with
    switch (config.which_timer) {
        case Timer_1:
            switch (config.divide) {
                case Div_1:
                    config.divide = 0b00;
                    break;
                case Div_8:
                    config.divide = 0b01;
                    break;
                case Div_64:
                    config.divide = 0b10;
                    break;
                case Div_256:
                    config.divide = 0b11;
                    break;
                default:
                    //set divide to 64? this is bad error handling
                    config.divide = 0b10;
                    break;
            }

            T1CONbits.TCKPS = config.divide; //set the clock divider
            PR1 = config.period; //set the period for the timer
            IPC1bits.T1IP = 7; //set the interrupt to priority level 7
            IEC0bits.T1IE = config.enabled; //enable the interrupt
            timer_1_callback = config.callback; //set the ISR function pointer
            T1CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_2:
            T2CONbits.TCKPS = config.divide; //set the clock divider
            PR2 = config.period; //set the period for the timer
            IPC2bits.T2IP = 7; //set the interrupt priority
            IEC0bits.T2IE = config.enabled; //set the interrupt enable
            timer_2_callback = config.callback; //set the ISR function pointer
            T2CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_3:
            T3CONbits.TCKPS = config.divide; //set the clock divider
            PR3 = config.period; //set the period of the timer
            IPC3bits.T3IP = 7; //set the interrupt priotity
            IEC0bits.T3IE = config.enabled; //enable the interrupt
            timer_3_callback = config.callback; //set the ISR function pointer
            T3CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_4:
            T4CONbits.TCKPS = config.divide;
            PR4 = config.period;
            IPC4bits.T4IP = 7;
            IEC0bits.T4IE = config.enabled;
            timer_4_callback = config.callback;
            T4CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_5:
            T5CONbits.TCKPS = config.divide;
            PR5 = config.period;
            IPC5bits.T5IP = 7;
            IEC0bits.T5IE = config.enabled;
            timer_5_callback = config.callback;
            T5CONbits.ON = config.enabled; //actually turn the timer on
            break;
    }
}

void enable_TIMER(Timer_Type which_timer)
{
    switch(which_timer)
    {
        case Timer_1:
            T1CONbits.ON = 1;
            break;
        case Timer_2:
            T2CONbits.ON = 1;
            break;
        case Timer_3:
            T3CONbits.ON = 1;
            break;
        case Timer_4:
            T4CONbits.ON = 1;
            break;
        case Timer_5:
            T5CONbits.ON = 1;
            break;
    }
}

void disable_TIMER(Timer_Type which_timer)
{
    switch(which_timer)
    {
        case Timer_1:
            T1CONbits.ON = 0;
            break;
        case Timer_2:
            T2CONbits.ON = 0;
            break;
        case Timer_3:
            T3CONbits.ON = 0;
            break;
        case Timer_4:
            T4CONbits.ON = 0;
            break;
        case Timer_5:
            T5CONbits.ON = 0;
            break;
    }
}

void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer_Handler_1(void) {
    asm volatile ("di"); //disable interrupt


    if (timer_1_callback != NULL) {
        timer_1_callback();
    }

    IFS0bits.T1IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_2_VECTOR, IPL7AUTO) Timer_Handler_2(void) {
    asm volatile ("di"); //display interrupts


    if (timer_2_callback != NULL) {
        timer_2_callback();
    }

    IFS0bits.T2IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer_Handler_3(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_3_callback != NULL) {
        timer_3_callback();
    }

    IFS0bits.T3IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_4_VECTOR, IPL7AUTO) Timer_Handler_4(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_4_callback != NULL) {
        timer_4_callback();
    }

    IFS0bits.T4IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_5_VECTOR, IPL7AUTO) Timer_Handler_5(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_5_callback != NULL) {
        timer_5_callback();
    }

    IFS0bits.T5IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}
