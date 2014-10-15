/********************************************************
 *   File Name: THRUSTER_TIMER_ISR.c
 *
 *   Description:
 *              source code file for the Thruster Timer ISR
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#if defined (COMPILE_THRUSTER_BOARD)
#include "THRUSTER_TIMER_ISR.h"
#include "comm_UART_ISR.h"
#include "GPIO.h"


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
void thruster_timer_setup(void) {
    THRUSTER_TIMER_PS = THRUSTER_TIMER_PRESCALER; //set the prescaler bits (1/256 in this case)
    THRUSTER_TIMER_PR = THRUSTER_TIMER_PERIOD_REGISTER; //set the period register

    THRUSTER_TIMER_INT_PRIORITY_set(7); //set the priority
    THRUSTER_TIMER_INT_set(1); //enable the interrupt

    THRUSTER_TIMER_EN = 1; //enable the timer
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void thruster_timer_begin(void) {
    THRUSTER_TIMER_IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_THRUSTER_TIMER_VECTOR, IPL7AUTO) thruster_timer_handler(void) {

    //extern boolean COMM_UART_is_idle;
    extern THRUSTER_STATUS Thruster_Status;
    static uint8 count;


    INTDisableInterrupts();

    count = count + 1;
    if (count > 126) {
        count = 0;
    }

    //Insert LED commands here - need pin mappings
    //Create PWM
    if (Thruster_Status.BOW_PORT_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        BOW_PORT_MAG_LED_PORT = 0;
        BOW_PORT_DIR_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.BOW_PORT_MAG) {
            //Turn on MAG LED
            BOW_PORT_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            BOW_PORT_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.BOW_PORT_DIR == 0) {
            //Turn off DIR LED
            BOW_PORT_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            BOW_PORT_DIR_LED_PORT = 1;
        }
    }

    if (Thruster_Status.BOW_SB_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        BOW_SB_MAG_LED_PORT = 0;
        BOW_SB_DIR_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.BOW_SB_MAG) {
            //Turn on MAG LED
            BOW_SB_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            BOW_SB_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.BOW_SB_DIR == 0) {
            //Turn off DIR LED
            BOW_SB_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            BOW_SB_DIR_LED_PORT = 1;
        }
    }

    if (Thruster_Status.STERN_PORT_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        STERN_PORT_DIR_LED_PORT = 0;
        STERN_PORT_MAG_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.STERN_PORT_MAG) {
            //Turn on MAG LED
            STERN_PORT_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            STERN_PORT_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.STERN_PORT_DIR == 0) {
            //Turn off DIR LED
            STERN_PORT_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            STERN_PORT_DIR_LED_PORT = 1;
        }
    }

    if (Thruster_Status.STERN_SB_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        STERN_SB_MAG_LED_PORT = 0;
        STERN_SB_DIR_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.STERN_SB_MAG) {
            //Turn on MAG LED
            STERN_SB_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            STERN_SB_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.STERN_SB_DIR == 0) {
            //Turn off DIR LED
            STERN_SB_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            STERN_SB_DIR_LED_PORT = 1;
        }
    }

    if (Thruster_Status.DEPTH_PORT_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        DEPTH_PORT_MAG_LED_PORT = 0;
        DEPTH_PORT_DIR_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.DEPTH_PORT_MAG) {
            //Turn on MAG LED
            DEPTH_PORT_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            DEPTH_PORT_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.DEPTH_PORT_DIR == 0) {
            //Turn off DIR LED
            DEPTH_PORT_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            DEPTH_PORT_DIR_LED_PORT = 1;
        }
    }

    if (Thruster_Status.DEPTH_SB_MAG == 0) {
        //Turn off both DIR and MAG LEDs
        DEPTH_SB_MAG_LED_PORT = 0;
        DEPTH_SB_DIR_LED_PORT = 0;
    } else {
        if (count < Thruster_Status.DEPTH_SB_MAG) {
            //Turn on MAG LED
            DEPTH_SB_MAG_LED_PORT = 1;
        } else {
            //Turn off MAG LED
            DEPTH_SB_MAG_LED_PORT = 0;
        }
        if (Thruster_Status.DEPTH_SB_DIR == 0) {
            //Turn off DIR LED
            DEPTH_SB_DIR_LED_PORT = 0;
        } else {
            //Turn on DIR LED
            DEPTH_SB_DIR_LED_PORT = 1;
        }
    }


    THRUSTER_TIMER_IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void thruster_LED_setup(void) {
    int init_val = 1;

    //set the tris registers to output
    BOW_SB_DIR_LED_TRIS = OUTPUT;
    BOW_SB_MAG_LED_TRIS = OUTPUT;
    BOW_PORT_DIR_LED_TRIS = OUTPUT;
    BOW_PORT_MAG_LED_TRIS = OUTPUT;
    STERN_SB_DIR_LED_TRIS = OUTPUT;
    STERN_SB_MAG_LED_TRIS = OUTPUT;
    STERN_PORT_DIR_LED_TRIS = OUTPUT;
    STERN_PORT_MAG_LED_TRIS = OUTPUT;
    DEPTH_SB_DIR_LED_TRIS = OUTPUT;
    DEPTH_SB_MAG_LED_TRIS = OUTPUT;
    DEPTH_PORT_DIR_LED_TRIS = OUTPUT;
    DEPTH_PORT_MAG_LED_TRIS = OUTPUT;
    
    //initialize all ports
    BOW_SB_DIR_LED_PORT = init_val;
    BOW_SB_MAG_LED_PORT = init_val;
    BOW_PORT_DIR_LED_PORT = init_val;
    BOW_PORT_MAG_LED_PORT = init_val;
    STERN_SB_DIR_LED_PORT = init_val;
    STERN_SB_MAG_LED_PORT = init_val;
    STERN_PORT_DIR_LED_PORT = init_val;
    STERN_PORT_MAG_LED_PORT = init_val;
    DEPTH_SB_DIR_LED_PORT = init_val;
    DEPTH_SB_MAG_LED_PORT = init_val;
    DEPTH_PORT_DIR_LED_PORT = init_val;
    DEPTH_PORT_MAG_LED_PORT = init_val;
}

#endif
