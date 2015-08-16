#include "PWM.h"
#include "Timer.h"
void (*PWM_1_callback)(void);
void (*PWM_2_callback)(void);
void (*PWM_3_callback)(void);
void (*PWM_4_callback)(void);
void (*PWM_5_callback)(void);

    void initialize_PWM(PWM_Config config) {
        //We will first initialize the OC channel
        switch (config.PWM_Channel) {
            case PWM_CH_1:
                switch (config.pin) {
                    case Pin_RPB3:
                        ANSELBbits.ANSB3 = 0;
                        TRISBbits.TRISB3 = 0;
                        RPB3R = 0b0101;
                        break;
                    case Pin_RPB4:
                        TRISBbits.TRISB4 = 0;
                        RPB4R = 0b0101;
                        break;
                    case Pin_RPB15:
                        ANSELBbits.ANSB15 = 0;
                        TRISBbits.TRISB15 = 0;
                        RPB15R = 0b0101;
                        break;
                    case Pin_RPB7:
                        TRISBbits.TRISB7 = 0;
                        RPB7R = 0b0101;
                        break;
                    default: //RPA0
                        ANSELAbits.ANSA0 = 0;
                        TRISAbits.TRISA0 = 0;
                        RPA0R = 0b0101;
                        break;
                }
                OC1CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC1CONbits.OCTSEL = 0;
                    OC1R = config.dutyCycle * PR2;
                    OC1RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC1CONbits.OCTSEL = 1;
                    OC1R = config.dutyCycle * PR3;
                    OC1RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC1CONbits.OCM = 0b110; //PWM no fault pin mode
                
                //if the callback is not null, there is a function to implement
                if (config.callback != NULL) {
                    PWM_1_callback = config.callback;
                    IPC1bits.OC1IP = 7;
                    IFS0bits.OC1IF = 0;
                    IEC0bits.OC1IE = 1;
                }
                OC1CONbits.ON = config.enable;
                break;
            case PWM_CH_2:
                switch (config.pin) {
                    case Pin_RPB5:
                        TRISBbits.TRISB5 = 0;
                        RPB5R = 0b0101;
                        break;
                    case Pin_RPB1:
                        ANSELBbits.ANSB1 = 0;
                        TRISBbits.TRISB1 = 0;
                        RPB1R = 0b0101;
                        break;
                    case Pin_RPB11:
                        TRISBbits.TRISB11 = 0;
                        RPB11R = 0b0101;
                        break;
                    case Pin_RPB8:
                        TRISBbits.TRISB8 = 0;
                        RPB8R = 0b0101;
                        break;
                    default: //RPA1
                        ANSELAbits.ANSA1 = 0;
                        TRISAbits.TRISA1 = 0;
                        RPA1R = 0b0101;
                        break;
                }
                OC2CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC2CONbits.OCTSEL = 0;
                    OC2R = config.dutyCycle * PR2;
                    OC2RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC2CONbits.OCTSEL = 1;
                    OC2R = config.dutyCycle * PR3;
                    OC2RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC2CONbits.OCM = 0b110; //PWM no fault pin mode
                
                //if the callback is not null, there is a function to implement
                if (config.callback != NULL) {
                    PWM_2_callback = config.callback;
                    IPC2bits.OC2IP = 7;
                    IFS0bits.OC2IF = 0;
                    IEC0bits.OC2IE = 1;
                }
                OC2CONbits.ON = config.enable;
                break;
            case PWM_CH_3:
                switch (config.pin) {
                    case Pin_RPB14:
                        ANSELBbits.ANSB14 = 0;
                        TRISBbits.TRISB14 = 0;
                        RPB14R = 0b0101;
                        break;
                    case Pin_RPB0:
                        ANSELBbits.ANSB0 = 0;
                        TRISBbits.TRISB0 = 0;
                        RPB0R = 0b0101;
                        break;
                    case Pin_RPB10:
                        TRISBbits.TRISB10 = 0;
                        RPB10R = 0b0101;
                        break;
                    case Pin_RPB9:
                        TRISBbits.TRISB9 = 0;
                        TRISBbits.TRISB9 = 0;
                        RPB9R = 0b0101;
                        break;
                    default: //RPA3
                        TRISAbits.TRISA3 = 0;
                        RPA3R = 0b0101;
                        break;
                }
                OC3CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC3CONbits.OCTSEL = 0;
                    OC3R = config.dutyCycle * PR2;
                    OC3RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC3CONbits.OCTSEL = 1;
                    OC3R = config.dutyCycle * PR3;
                    OC3RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC3CONbits.OCM = 0b110; //PWM no fault pin mode
                
                //if the callback is not null, there is a function to implement
                if (config.callback != NULL) {
                    PWM_3_callback = config.callback;
                    IPC3bits.OC3IP = 7;
                    IFS0bits.OC3IF = 0;
                    IEC0bits.OC3IE = 1;
                }
                OC3CONbits.ON = config.enable;
                break;
            case PWM_CH_4:
                switch (config.pin) {
                    case Pin_RPB6:
                        //error, the xc32 compiler doesnt support this for some reason
                        break;
                    case Pin_RPA4:
                        TRISAbits.TRISA4 = 0;
                        RPA4R = 0b0101;
                        break;
                    case Pin_RPB13:
                        TRISBbits.TRISB13 = 0;
                        RPB13R = 0b0101;
                        break;
                    case Pin_RPB2:
                        TRISBbits.TRISB2 = 0;
                        TRISBbits.TRISB2 = 0;
                        RPB2R = 0b0101;
                        break;
                    default: //RPA2
                        TRISAbits.TRISA2 = 0;
                        RPA2R = 0b0101;
                        break;
                }
                OC4CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC4CONbits.OCTSEL = 0;
                    OC4R = config.dutyCycle * PR2;
                    OC4RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC4CONbits.OCTSEL = 1;
                    OC4R = config.dutyCycle * PR3;
                    OC4RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC4CONbits.OCM = 0b110; //PWM no fault pin mode
                
                //if the callback is not null, there is a function to implement
                if (config.callback != NULL) {
                    PWM_4_callback = config.callback;
                    IPC4bits.OC4IP = 7;
                    IFS0bits.OC4IF = 0;
                    IEC0bits.OC4IE = 1;
                }
                OC4CONbits.ON = config.enable;
                break;
            case PWM_CH_5:
                switch (config.pin) {
                    case Pin_RPB6:
                        //error, the xc32 compiler doesnt support this for some reason
                        break;
                    case Pin_RPA4:
                        TRISAbits.TRISA4 = 0;
                        RPA4R = 0b0110;
                        break;
                    case Pin_RPB13:
                        TRISBbits.TRISB13 = 0;
                        RPB13R = 0b0110;
                        break;
                    case Pin_RPB2:
                        TRISBbits.TRISB2 = 0;
                        TRISBbits.TRISB2 = 0;
                        RPB2R = 0b0110;
                        break;
                    default: //RPA2
                        TRISAbits.TRISA2 = 0;
                        RPA2R = 0b0110;
                        break;
                }
                OC5CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC5CONbits.OCTSEL = 0;
                    OC5R = config.dutyCycle * PR2;
                    OC5RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC5CONbits.OCTSEL = 1;
                    OC5R = config.dutyCycle * PR3;
                    OC5RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC5CONbits.OCM = 0b110; //PWM no fault pin mode
                
                //if the callback is not null, there is a function to implement
                if (config.callback != NULL) {
                    PWM_5_callback = config.callback;
                    IPC5bits.OC5IP = 7;
                    IFS0bits.OC5IF = 0;
                    IEC0bits.OC5IE = 1;
                }
                OC5CONbits.ON = config.enable;
                break;
        }
    }
    
    void enable_PWM(PWM_Config config) {
        switch (config.channel) {
            case PWM_CH_1:
                OC1CONbits.ON = 1;
                if (config.callback != NULL) {
                    PWM_1_callback = config.callback;
                    IPC1bits.OC1IP = 7;
                    IFS0bits.OC1IF = 0;
                    IEC0bits.OC1IE = 1;
                }
                break;
            case PWM_CH_2:
                OC2CONbits.ON = 1;
                if (config.callback != NULL) {
                    PWM_2_callback = config.callback;
                    IPC2bits.OC2IP = 7;
                    IFS0bits.OC2IF = 0;
                    IEC0bits.OC2IE = 1;
                }
                break;
            case PWM_CH_3:
                OC3CONbits.ON = 1;
                if (config.callback != NULL) {
                    PWM_3_callback = config.callback;
                    IPC3bits.OC3IP = 7;
                    IFS0bits.OC3IF = 0;
                    IEC0bits.OC3IE = 1;
                }
                break;
            case PWM_CH_4:
                OC4CONbits.ON = 1;
                if (config.callback != NULL) {
                    PWM_4_callback = config.callback;
                    IPC4bits.OC4IP = 7;
                    IFS0bits.OC4IF = 0;
                    IEC0bits.OC4IE = 1;
                }
                break;
            case PWM_CH_5:
                OC5CONbits.ON = 1;
                if (config.callback != NULL) {
                    PWM_5_callback = config.callback;
                    IPC5bits.OC5IP = 7;
                    IFS0bits.OC5IF = 0;
                    IEC0bits.OC5IE = 1;
                }
                break;
        }
    }
    
    void disable_PWM(PWM_Config config) {
        switch (config.channel) {
            case PWM_CH_1:
                OC1CONbits.ON = 0;
                IEC0bits.OC1IE = 0;
                break;
            case PWM_CH_2:
                OC2CONbits.ON = 0;
                IEC0bits.OC2IE = 0;
                break;
            case PWM_CH_3:
                OC3CONbits.ON = 0;
                IEC0bits.OC3IE = 0;
                break;
            case PWM_CH_4:
                OC4CONbits.ON = 0;
                IEC0bits.OC4IE = 0;
                break;
            case PWM_CH_5:
                OC5CONbits.ON = 0;
                IEC0bits.OC5IE = 0;
                break;
        }
    }
    
    void update_PWM(PWM_Config config, float dutyCycle) {
        //update the duty cycle of the respective PWM
        switch (config.channel) {
            case PWM_CH_1:
                if (config.timer.which_timer == Timer_2) {
                    OC1RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC1RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_2:
                if (config.timer.which_timer == Timer_2) {
                    OC2RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC2RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_3:
                if (config.timer.which_timer == Timer_2) {
                    OC3RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC3RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_4:
                if (config.timer.which_timer == Timer_2) {
                    OC4RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC4RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_5:
                if (config.timer.which_timer == Timer_2) {
                    OC5RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC5RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
        }
    }

    void __ISR(_OUTPUT_COMPARE_1_VECTOR, IPL7AUTO) OC1_Handler(void) 
    {
        asm volatile ("di");
        
        if (PWM_1_callback != NULL) {
            PWM_1_callback();
        }
        
        asm volatile ("ei");
    }
    void __ISR(_OUTPUT_COMPARE_2_VECTOR, IPL7AUTO) OC2_Handler(void) 
    {
        asm volatile ("di");
        
        if (PWM_2_callback != NULL) {
            PWM_2_callback();
        }
        
        asm volatile ("ei");
    }
    
    
    void __ISR(_OUTPUT_COMPARE_3_VECTOR, IPL7AUTO) OC3_Handler(void) 
    {
        asm volatile ("di");
        
        if (PWM_3_callback != NULL) {
            PWM_3_callback();
        }
        
        asm volatile ("ei");
    }
    
    
    void __ISR(_OUTPUT_COMPARE_4_VECTOR, IPL7AUTO) OC4_Handler(void) 
    {
        asm volatile ("di");
        
        if (PWM_4_callback != NULL) {
            PWM_4_callback();
        }
        
        asm volatile ("ei");
    }
    
    void __ISR(_OUTPUT_COMPARE_5_VECTOR, IPL7AUTO) OC5_Handler(void) 
    {
        asm volatile ("di");
        
        if (PWM_5_callback != NULL) {
            PWM_5_callback();
        }
        
        asm volatile ("ei");
    }
    