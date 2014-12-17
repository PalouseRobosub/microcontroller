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
    Div_8,
    Div_64,
    Div_256
}Clock_Divider;

typedef enum {
    Timer_1,
    Timer_2,
    Timer_3,
    Timer_4,
    Timer_5
}Timer_Type;

/* #Define Functions / Inline Functions*/
    //begin

/* Function Prototypes*/
    //setup
    void timer_setup(Clock_Divider divide, uint16 period, Timer_Type timer, void *function_ptr);



#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

