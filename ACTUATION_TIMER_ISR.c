
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
#if defined (COMPILE_ACTUATION_BOARD)
#include "ACTUATION_TIMER_ISR.h"
#include "comm_UART_ISR.h"
#include "GPIO.h"


/*************************************************************************
 Variables
 ************************************************************************/

/* START FUNCTION DESCRIPTION ********************************************
Front stepper interrupt
END DESCRIPTION **********************************************************/

void __ISR(_ACTUATION_TIMER_VECTOR, ipl2) actuation_timer_handler(void) //Stepper interrupt
{
    //FRONT STEPPER VARIABLES (f = FRONT)
    //Internal
    extern int Fpos_goal; //Desired pos_current
    extern int Fpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    extern int Fsteps; //keeps track of total number of steps
    extern int Freset; //Reset flag
    extern int Fdir; //Direction (OPEN or CLOSE)
    extern int Foutput; //Used to output bits to stepper motor
    extern int Fstepper_en; //Half H-bridge enable
    extern int Fstep_delay;
    extern int Ftrigger;

    extern int Bpos_goal; //Desired pos_current
    extern int Bpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    extern int Bsteps; //keeps track of total number of steps
    extern int Breset; //Reset flag
    extern int Bdir; //Direction (OPEN or CLOSE)
    extern int Boutput; //Used to output bits to stepper motor
    extern int Bstepper_en; //Half H-bridge enable
    extern int Bstep_delay;
    extern int Btrigger;

    INTDisableInterrupts();

    //BEGINNING OF FRONT STEPPER MOTOR SCRIPT
    Fpos_current = Fsteps / STEPS_PER_POS; //translates total steps into pos_current

    if (Fpos_goal == Fpos_current) {
        Fstepper_en = EN_OFF;
    } else {
        Fstepper_en = EN_ON;
    }

    //Movement commands
    //Step delay causes one movement every step_delay interrupts
    if (Fstep_delay == 0) {
        Fstep_delay = STEP_PERIOD;

        if (Freset) //if reset rotates motor close until latch is triggered
        {
            if (Ftrigger) {
                Freset = 0;
                Fsteps = 0;
                Fpos_goal = 0;
            } else {
                Fdir = DIR_OPEN;
                Foutput = stepper_state_machine(Fdir, FRONT_STEPPER); //direction = open (100% => zero position)
                output_to_stepper_motor(Foutput, FRONT_STEPPER);
                Fsteps--;
            }
        } else {
            if (Fpos_current < Fpos_goal) //if true, rotate close one step
            {
                Fdir = DIR_CLOSED;
                Foutput = stepper_state_machine(Fdir, FRONT_STEPPER);
                output_to_stepper_motor(Foutput, FRONT_STEPPER);
                Fsteps++;
            }

            if (Fpos_current > Fpos_goal) //if true, rotate open one step
            {
                if (Ftrigger) //stops movement until next command if latch is triggered
                {
                    Fsteps = 0;
                    Fpos_goal = 0;
                } else {
                    Fdir = DIR_OPEN;
                    Foutput = stepper_state_machine(Fdir, FRONT_STEPPER);
                    output_to_stepper_motor(Foutput, FRONT_STEPPER);
                    Fsteps--;
                }
            }
        }

    }

    Fstep_delay--; //Decrement step delay

    ACTUATION_TIMER_IF = 0; //Clear timer interrupt flag
    INTEnableInterrupts(); //Re-enable interrupts

}

#endif
