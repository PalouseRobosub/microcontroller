/********************************************************
 *   File Name: GPIO.c
 *
 *   Description:
 *              source code file for GPIO
 *
 *
 *********************************************************/
#include "system.h"
#include "GPIO.h"

#if defined(COMPILE_OLD_SUB)
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
void GPIO_setup(void) {

    //setup pneumatics tris registers
    PNEUMATIC_TORPEDO_R_TRIS = 0;
    PNEUMATIC_TORPEDO_L_TRIS = 0;
    PNEUMATIC_MARKER_R_TRIS = 0;
    PNEUMATIC_MARKER_L_TRIS = 0;
    PNEUMATIC_CLAW_OPEN_TRIS = 0;
    PNEUMATIC_CLAW_CLOSE_TRIS = 0;

    //set all pins to 0
    PNEUMATIC_TORPEDO_R_PIN = 0;
    PNEUMATIC_TORPEDO_L_PIN = 0;
    PNEUMATIC_MARKER_R_PIN = 0;
    PNEUMATIC_MARKER_L_PIN = 0;
    PNEUMATIC_CLAW_OPEN_PIN = 0;
    PNEUMATIC_CLAW_CLOSE_PIN = 0;
}

#elif defined (COMPILE_ACTUATION_BOARD)
/*************************************************************************
 Variables
 ************************************************************************/
int Fpos_goal; //Desired pos_current
int Fpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
int Fsteps; //keeps track of total number of steps
int Freset; //Reset flag
int Fdir; //Direction (OPEN or CLOSE)
int Foutput; //Used to output bits to stepper motor
int Fstepper_en; //Half H-bridge enable
int Fstep_delay;

int Bpos_goal; //Desired pos_current
int Bpos_current; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
int Bsteps; //keeps track of total number of steps
int Breset; //Reset flag
int Bdir; //Direction (OPEN or CLOSE)
int Boutput; //Used to output bits to stepper motor
int Bstepper_en; //Half H-bridge enable
int Bstep_delay;

int stepper_command, Btrigger, Ftrigger;

/* START FUNCTION DESCRIPTION ********************************************
sys_init                <Tester.c>
SYNTAX:       		void sys_init(void);
KEYWORDS:     		system, initialize
DESCRIPTION:  		Initializes system
RETURN VALUE: 		None.
END DESCRIPTION **********************************************************/
void sys_init(void) {
    stepper_command = STOP_COMMAND;
    Ftrigger = 0;
    Fpos_goal = 0; //Desired pos_current
    Fpos_current = 0; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    Fsteps = 0; //keeps track of total number of steps
    Freset = 0; //Reset flag
    Fdir = DIR_CLOSED; //Direction (OPEN or CLOSE)
    Foutput = 0; //Used to output bits to stepper motor
    Fstepper_en = 0; //Half H-bridge enable
    Fstep_delay = 0;

    Btrigger = 0;
    Bpos_goal = 0; //Desired pos_current
    Bpos_current = 0; //Keeps track of steps/STEPS_PER_COUNT to compare to pos_goal
    Bsteps = 0; //keeps track of total number of steps
    Breset = 0; //Reset flag
    Bdir = DIR_CLOSED; //Direction (OPEN or CLOSE)
    Boutput = 0; //Used to output bits to stepper motor
    Bstepper_en = 0; //Half H-bridge enable
    Bstep_delay = 0;

    DDPCONbits.JTAGEN = 0;

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
        //LATB = (command << 7) | (LATB & ~STEPPER_MASK);
    }

    if (which_stepper == BOTTOM_STEPPER) {
        //Down stepper command bits TBD
    }
}

#endif
