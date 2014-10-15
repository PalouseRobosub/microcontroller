
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
//torpedo state variables
int torpedo_L_fire;
int torpedo_R_fire;

//Stepper motor state variables
int Fpos_goal; //Desired pos_current
int Fpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
int Fsteps; //keeps track of total number of steps
int Freset; //Reset flag
int Fdir; //Direction (OPEN or CLOSE)
int Foutput; //Used to output bits to stepper motor

int Bpos_goal; //Desired pos_current
int Bpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
int Bsteps; //keeps track of total number of steps
int Breset; //Reset flag
int Bdir; //Direction (OPEN or CLOSE)
int Boutput; //Used to output bits to stepper motor

int stepper_command;

/********************************************************
 *   Function Name: torpedo_setup
 *
 *   Description:
 *
 *
 *********************************************************/
void torpedo_setup(void) {
    torpedo_R_fire = 0;
    torpedo_L_fire = 0;
}

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

void __ISR(_ACTUATION_TIMER_VECTOR, IPL7AUTO) actuation_timer_handler(void) //Stepper interrupt
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
    //
    //pneumatic logic
    //

    //right torpedo
    if (torpedo_R_fire == 0)
    {
        PNEUMATIC_TORPEDO_R_PIN = 0;
    }
    else
    {
        PNEUMATIC_TORPEDO_R_PIN = 1;
        ++torpedo_R_fire;
        if (torpedo_R_fire > TORPEDO_DURATION)
        {
            torpedo_R_fire = 0;
        }
    }
    
    //left torpedo
    if (torpedo_L_fire == 0)
    {
        PNEUMATIC_TORPEDO_L_PIN = 0;
    }
    else
    {
        PNEUMATIC_TORPEDO_L_PIN = 1;
        ++torpedo_L_fire;
        if (torpedo_L_fire > TORPEDO_DURATION)
        {
            torpedo_L_fire = 0;
        }
    }

    //BEGINNING OF FRONT STEPPER MOTOR SCRIPT
    Fpos_current = Fsteps / STEPS_PER_POS; //translates total steps into pos_current
    Bpos_current = Bsteps / STEPS_PER_POS; //translates total steps into pos_current

    //Movement commands
    //Front motor
        if (Freset) //if reset rotates motor close until latch is triggered
        {
            if (!SEN1_PIN) {  //Sensor trigger 1
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
                if (!SEN1_PIN) //stops movement until next command if latch is triggered
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
            if (!SEN2_PIN) {  //Sensor trigger 2
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
                if (!SEN2_PIN) //stops movement until next command if latch is triggered
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

/* START FUNCTION DESCRIPTION ********************************************
sys_init                <Tester.c>
SYNTAX:       		void sys_init(void);
KEYWORDS:     		system, initialize
DESCRIPTION:  		Initializes system
RETURN VALUE: 		None.
END DESCRIPTION **********************************************************/
void stepper_motor_setup(void) {
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

    extern int stepper_command;



    stepper_command = STOP_COMMAND;
    Fpos_goal = 0; //Desired pos_current
    Fpos_current = 0; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    Fsteps = 0; //keeps track of total number of steps
    Freset = 0; //Reset flag
    Fdir = DIR_CLOSED; //Direction (OPEN or CLOSE)
    Foutput = 0; //Used to output bits to stepper motor

    Bpos_goal = 0; //Desired pos_current
    Bpos_current = 0; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    Bsteps = 0; //keeps track of total number of steps
    Breset = 0; //Reset flag
    Bdir = DIR_CLOSED; //Direction (OPEN or CLOSE)
    Boutput = 0; //Used to output bits to stepper motor


}

/* START FUNCTION DESCRIPTION ********************************************
stepper_state_machine
SYNTAX:       		int stepper_state_machine(int code);
PARAMETER2:			dir: CW, CCW
KEYWORDS:     		state machine, stepper
DESCRIPTION:  		Changes static state variable based on mode and dir
                                        and returns motor command
RETURN VALUE: 		int command[state]
END DESCRIPTION **********************************************************/
int stepper_state_machine(int dir, int which_stepper) {
    static enum STEPPER_STATE front_state = S_0_5;
    static enum STEPPER_STATE bottom_state = S_0_5;

    static const unsigned int command[8] = {0x0A, 0x08, 0x09, 0x01, 0x05, 0x04, 0x06, 0x02};

    if (which_stepper == FRONT_STEPPER) {
        if (dir == DIR_OPEN) {
            front_state = (front_state + 1) % 8;
        }
        if (dir == DIR_CLOSED) {
            front_state = (front_state + 7) % 8;
        }

        //This returns the appropriate motor command for that state
        return command[front_state];
    }

    if (which_stepper == BOTTOM_STEPPER) {
        if (dir == DIR_OPEN) {
            bottom_state = (bottom_state + 1) % 8;
        }
        if (dir == DIR_CLOSED) {
            bottom_state = (bottom_state + 7) % 8;
        }

        //This returns the appropriate motor command for that state
        return command[bottom_state];
    }
}

/* START FUNCTION DESCRIPTION ********************************************
output_to_stepper_motor
SYNTAX:       		void output_to_stepper_motor(int command);
PARAMETER1:			command
KEYWORDS:     		output, stepper, motor
DESCRIPTION:  		Sends command to stepper motor
RETURN VALUE: 		None.
END DESCRIPTION **********************************************************/
void output_to_stepper_motor(int command, int which_stepper) {
    if (which_stepper == FRONT_STEPPER) {
        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN12_PIN = !!(command & 0b1000);
        STEP_IN11_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        ///*
        STEP_IN12_PIN = !!(command & 0b1000);
        STEP_IN13_PIN = !!(command & 0b0100);
        STEP_IN11_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        //*/
        
        /*
        STEP_IN12_PIN = !!(command & 0b1000);
        STEP_IN13_PIN = !!(command & 0b0100);
        STEP_IN14_PIN = !!(command & 0b0010);
        STEP_IN11_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN13_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN14_PIN = !!(command & 0b0010);
        STEP_IN11_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN13_PIN = !!(command & 0b1000);
        STEP_IN14_PIN = !!(command & 0b0100);
        STEP_IN12_PIN = !!(command & 0b0010);
        STEP_IN11_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN13_PIN = !!(command & 0b1000);
        STEP_IN14_PIN = !!(command & 0b0100);
        STEP_IN11_PIN = !!(command & 0b0010);
        STEP_IN12_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN14_PIN = !!(command & 0b1000);
        STEP_IN13_PIN = !!(command & 0b0100);
        STEP_IN11_PIN = !!(command & 0b0010);
        STEP_IN12_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN14_PIN = !!(command & 0b1000);
        STEP_IN11_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN12_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN14_PIN = !!(command & 0b1000);
        STEP_IN11_PIN = !!(command & 0b0100);
        STEP_IN12_PIN = !!(command & 0b0010);
        STEP_IN13_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN14_PIN = !!(command & 0b0100);
        STEP_IN12_PIN = !!(command & 0b0010);
        STEP_IN13_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN14_PIN = !!(command & 0b0010);
        STEP_IN13_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */

        /*
        STEP_IN11_PIN = !!(command & 0b1000);
        STEP_IN12_PIN = !!(command & 0b0100);
        STEP_IN13_PIN = !!(command & 0b0010);
        STEP_IN14_PIN = !!(command & 0b0001);
        */


    }

    if (which_stepper == BOTTOM_STEPPER) {

//        STEP_IN22_PIN = !!(command & 0b1000);
//        STEP_IN23_PIN = !!(command & 0b0100);
//        STEP_IN21_PIN = !!(command & 0b0010);
//        STEP_IN24_PIN = !!(command & 0b0001);

        STEP_IN24_PIN = !!(command & 0b1000);
        STEP_IN21_PIN = !!(command & 0b0100);
        STEP_IN23_PIN = !!(command & 0b0010);
        STEP_IN22_PIN = !!(command & 0b0001);


//        STEP_IN21_PIN = !!(command & 0b1000);
//        STEP_IN22_PIN = !!(command & 0b0100);
//        STEP_IN23_PIN = !!(command & 0b0010);
//        STEP_IN24_PIN = !!(command & 0b0001);
    }
}

#endif
