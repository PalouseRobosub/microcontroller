
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

    //BEGINNING OF FRONT STEPPER MOTOR SCRIPT
    Fpos_current = Fsteps / STEPS_PER_POS; //translates total steps into pos_current
    Bpos_current = Bsteps / STEPS_PER_POS; //translates total steps into pos_current

    //Movement commands
    //Front motor
        if (Freset) //if reset rotates motor close until latch is triggered
        {
            if (SEN1_PIN) {  //Sensor trigger 1
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
                if (SEN1_PIN) //stops movement until next command if latch is triggered
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
            if (SEN1_PIN) {  //Sensor trigger 1
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
                if (SEN1_PIN) //stops movement until next command if latch is triggered
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

    DDPCONbits.JTAGEN = 0; //JMI: what does this do? Can we cut it?

    //TRISGSET = (BTN1 | BTN2);
    //TRISBCLR = (STEPPER_MASK | LEDA | LEDB| LEDC);

    //LATB = LATB & ~(STEPPER_MASK | LEDA | LEDB| LEDC);

    //CN Interrupt
    //mCNOpen((CN_ON | CN_FRZ_OFF | CN_IDLE_STOP),(CN8_ENABLE | CN9_ENABLE), CN_PULLUP_DISABLE_ALL);
    //ConfigIntCN(CHANGE_INT_ON | CHANGE_INT_PRI_1);

    //Timer1 Interrupt (1ms)

    //SET THIS

    //OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, TCKS_PER_MS);
    //ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

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

        STEP_IN11_PIN = command & 0b1000;
        STEP_IN12_PIN = command & 0b0100;
        STEP_IN13_PIN = command & 0b0010;
        STEP_IN14_PIN = command & 0b0001;
        //LATB = (command << 7) | (LATB & ~STEPPER_MASK);
    }

    if (which_stepper == BOTTOM_STEPPER) {
        STEP_IN21_PIN = command & 0b1000;
        STEP_IN22_PIN = command & 0b0100;
        STEP_IN23_PIN = command & 0b0010;
        STEP_IN24_PIN = command & 0b0001;
    }
}

#endif
