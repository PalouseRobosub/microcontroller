/********************************************************
 *   File Name: GPIO.c
 *
 *   Description:
 *              source code file for GPIO
 *
 *
 *********************************************************/


/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
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
void GPIO_setup(void)
{

#if defined (COMPILE_OLD_SUB)
    //setup pneumatics tris registers
    PNEUMATIC_TORPEDO_R_TRIS = OUTPUT;
    PNEUMATIC_TORPEDO_L_TRIS = OUTPUT;
    PNEUMATIC_MARKER_R_TRIS = OUTPUT;
    PNEUMATIC_MARKER_L_TRIS = OUTPUT;
    PNEUMATIC_CLAW_OPEN_TRIS = OUTPUT;
    PNEUMATIC_CLAW_CLOSE_TRIS = OUTPUT;

    //set all pins to 0
    PNEUMATIC_TORPEDO_R_PIN = 0;
    PNEUMATIC_TORPEDO_L_PIN = 0;
    PNEUMATIC_MARKER_R_PIN = 0;
    PNEUMATIC_MARKER_L_PIN = 0;
    PNEUMATIC_CLAW_OPEN_PIN = 0;
    PNEUMATIC_CLAW_CLOSE_PIN = 0;
#endif

#if defined (COMPILE_ACTUATION_BOARD)

    //setup outputs and inputs
    //OUTPUTS
    PNEUMATIC_TORPEDO_R_TRIS = OUTPUT;
    PNEUMATIC_TORPEDO_L_TRIS = OUTPUT;

    //INPUTS
    SEN1_TRIS = INPUT;
    SEN2_TRIS = INPUT;
    STEP_IN11_TRIS = OUTPUT;
    STEP_IN12_TRIS = OUTPUT;
    STEP_IN13_TRIS = OUTPUT;
    STEP_IN14_TRIS = OUTPUT;
    STEP_IN21_TRIS = OUTPUT;
    STEP_IN22_TRIS = OUTPUT;
    STEP_IN23_TRIS = OUTPUT;
    STEP_IN24_TRIS = OUTPUT;
    STEP_EN_TRIS = OUTPUT;



    //Initialize Outputs
    PNEUMATIC_TORPEDO_R_PIN = 0;
    PNEUMATIC_TORPEDO_L_PIN = 0;
#endif

}
