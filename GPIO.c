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
    PNEUMATIC_TORPEDO_R_TRIS    = 0;
    PNEUMATIC_TORPEDO_L_TRIS    = 0;
    PNEUMATIC_MARKER_R_TRIS     = 0;
    PNEUMATIC_MARKER_L_TRIS     = 0;
    PNEUMATIC_CLAW_OPEN_TRIS    = 0;
    PNEUMATIC_CLAW_CLOSE_TRIS   = 0;

    //set all pins to 0
    PNEUMATIC_TORPEDO_R_PIN     = 0;
    PNEUMATIC_TORPEDO_L_PIN     = 0;
    PNEUMATIC_MARKER_R_PIN      = 0;
    PNEUMATIC_MARKER_L_PIN      = 0;
    PNEUMATIC_CLAW_OPEN_PIN     = 0;
    PNEUMATIC_CLAW_CLOSE_PIN    = 0;
}




#endif
