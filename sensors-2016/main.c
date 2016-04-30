#include "Sensors.h"
#include "sublibinal.h"

    //select programming pins
    #pragma config ICESEL = ICS_PGx3// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)

    // DEVCFG3
    // USERID = No Setting
    #pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
    #pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
    #pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
    #pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

    // DEVCFG2
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
    #pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
    #pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
    #pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
    #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 1)

    // DEVCFG1
    #pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
    #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
    #pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
    #pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
    #pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
    #pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
    #pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
    #pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
    #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
    #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
    #pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

    // DEVCFG0
    #pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
    //programming pins are selected above
    #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
    #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
    #pragma config CP = OFF                 // Code Protect (Protection Disabled)

    #ifndef PB_CLK
        #define PB_CLK 15000000
    #endif

char read = 0;
char contended = 0;

extern I2C_Node depth_prep;

int main()
{
    int i = 0;
    ANSELA = ANSELB = 0;
    
    TRISBbits.TRISB15 = 0;
    LATBbits.LATB15 = 1; //I2C is reset
    
    
    
    TRISAbits.TRISA3 = 0;
    LATAbits.LATA3 = 0;
    
    asm volatile("di"); //Disable interrupts
    
    configureTimer(); //Configure the timer that we read the sensors at and the wait timer for depth conversion
    configureSerial(); //Configure our serial communication to the host computer
    configureI2C(); //Configure I2C transactions
    
    LATBbits.LATB15 = 0; //I2C is not reset
    
    //delay to allow sensors to power up before configuring
    for(i=0; i < 1000000; ++i); //This delay is necessary for magnometer
    
    enable_Interrupts(); //Globally enable interrupts
    
    configureSensors(); //Configure sensors on startup.
    
    enable_Timer(READ_TIMER); //Enable the sensor read timer for sensor data acqusition
    enable_Timer(RESET_TIMER); //Enable the reset timer to notice bus contention
    
    for (i = 0; i < 4; i++)
    {
        depth_prep.device_id = SID_DEPTH_1 + i;
        if (i == 3)
        {
            switchChannel(i);
            send_I2C(I2C_CH_1, depth_prep); //Begin back and forth continuous depth/temp reads
        }
    }
    enable_Timer(WAIT_TIMER);
    
    while (1) //Enter the embedded loop
    {
        bg_process_packetizer(PACKET_UART_CH_1);
        
        if (read)
        {
            if (!contended)
            {
                for (i = 0; i < 4; i++)
                    readSensors(i);
            }
            read = 0;
        }
        bg_process_I2C(I2C_CH_1, FALSE);
    }
    
}