/* 
 * File:   Timer.h
 * Author: Ryan Summers
 *
 * Created on December 17, 2014, 12:36 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"

    typedef enum {
        Div_1,
        Div_2,
        Div_4,
        Div_8,
        Div_16,
        Div_32,
        Div_64,
        Div_256
    } Clock_Divider;

    typedef enum {
        Timer_1,
        Timer_2,
        Timer_3,
        Timer_4,
        Timer_5
    } Timer_Type;

    typedef struct TIMER_CONFIG {
        Timer_Type which_timer;
        Clock_Divider divide;
        uint16 period;
        //uint pb_clk;  //for future work
        //uint frequency;
        void (*callback);
        boolean enabled;
    }Timer_Config;


    /* #Define Functions / Inline Functions*/
    //This function is the ISR #define function template
    /* #define Timer_ISR_(x) void __ISR(_Timer_x_Vector, IPL7AUTO) Timer_Handler_x (void) {\
         INTDisableInterrupts();\
         if (timer_x_callback != NULL) {\
             timer_x_callback();\
         }\
         IFS0bits.TxIF = 0;\
         INTENableInterrupts();\
     }*/


    /* Function Prototypes*/
    //setup
    void initialize_timer(Timer_Config config);

    void enable_timer(Timer_Type which_timer);
    void disable_timer(Timer_Type which_timer);




#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

