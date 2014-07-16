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

#if defined (COMPILE_SENSOR_BOARD)
    //setup the pin as an input
    START_SW_TRIS = INPUT;
    
    //setup pin change interrrupts
    CHANGE_NOTICE_ON = 1; //enable the change notice peripheral module
       
    CHANGE_NOTICE_INT_set(1); //enable interrupt
    CHANGE_NOTICE_INT_PRIORITY_set(7); //set priority
    CHANGE_NOTICE_IF = 0; //clear the interrupt flag
    
    
    //select which pins will activate a change notice interrupt
    START_SW_CN_PIN_EN = 1; //enable change notice for the mission start switch
    
    
    
    //Read corresponding PORTx registers to clear mismatch condition on CN input pins.
    
#endif

}

/********************************************************
 *   Function Name: mission_start_Handler
 *
 *   Description:
 *             ISR for the mission start switch
 *
 *********************************************************/
void __ISR(_MISSION_START_VECTOR, IPL7AUTO) mission_start_Handler(void) {

    INTDisableInterrupts();
    
    if (START_SW_PIN == 1) //if the value is 1, then we are seeing a rising edge
    {
        comm_uart_CreateNode(/*insert parameters*/ ); //tell the computer we saw a rising edge 
    } //else: falling edge, we don't care
    
    
    CHANGE_NOTICE_IF = 0; //clear the interrupt flag
    
    INTEnableInterrupts();
}
