
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
/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void actuation_timer_setup(void) {
    ACTUATION_TIMER_PS = ACTUATION_TIMER_PRESCALER; //set the prescaler bits (1/256 in this case)
    ACTUATION_TIMER_PR = ACTUATION_TIMER_PERIOD_REGISTER; //set the period register

    ACTUATION_TIMER_INT_PRIORITY_set(7); //set the priority
    ACTUATION_TIMER_INT_set(1); //enable the interrupt

    ACTUATION_TIMER_EN = 1; //enable the timer
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void actuation_timer_begin(void) {
    ACTUATION_TIMER_IF = 1; //set the interrupt flag
}
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

    extern int Bpos_goal; //Desired pos_current
    extern int Bpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    extern int Bsteps; //keeps track of total number of steps
    extern int Breset; //Reset flag
    extern int Bdir; //Direction (OPEN or CLOSE)
    extern int Boutput; //Used to output bits to stepper motor

    INTDisableInterrupts();

    //BEGINNING OF FRONT STEPPER MOTOR SCRIPT
    Fpos_current = Fsteps / STEPS_PER_POS; //translates total steps into pos_current
    Bpos_current = Bsteps / STEPS_PER_POS; //translates total steps into pos_current

    //Movement commands
    //Front motor
        if (Freset) //if reset rotates motor close until latch is triggered
        {
            if (SEN1) {  //Sensor trigger 1
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
                if (SEN1) //stops movement until next command if latch is triggered
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

    
    //Bottom motor
    if (Breset) //if reset rotates motor close until latch is triggered
        {
            if (SEN1) {  //Sensor trigger 1
                Breset = 0;
                Bsteps = 0;
                Bpos_goal = 0;
            } else {
                Bdir = DIR_OPEN;
                Boutput = stepper_state_machine(Bdir, BOTTOM_STEPPER); //direction = open (100% => zero position)
                output_to_stepper_motor(Boutput, BOTTOM_STEPPER);
                Bsteps--;
            }
        } else {
            if (Bpos_current < Bpos_goal) //if true, rotate close one step
            {
                Bdir = DIR_CLOSED;
                Boutput = stepper_state_machine(Bdir, BOTTOM_STEPPER);
                output_to_stepper_motor(Boutput, BOTTOM_STEPPER);
                Bsteps++;
            }

            if (Bpos_current > Bpos_goal) //if true, rotate open one step
            {
                if (SEN1) //stops movement until next command if latch is triggered
                {
                    Bsteps = 0;
                    Bpos_goal = 0;
                } else {
                    Bdir = DIR_OPEN;
                    Boutput = stepper_state_machine(Bdir, BOTTOM_STEPPER);
                    output_to_stepper_motor(Boutput, BOTTOM_STEPPER);
                    Bsteps--;
                }
            }
        }

    ACTUATION_TIMER_IF = 0; //Clear timer interrupt flag
    INTEnableInterrupts(); //Re-enable interrupts

}

#endif
