#include "Sensors.h"
#include "sublibinal.h"

char read = 0;

int main()
{
    asm volatile("di"); //Disable interrupts
    
    configureTimer(); //Configure the timer that we read the sensors at and the wait timer for depth conversion
    configureSerial(); //Configure our serial communication to the host computer
    configureI2C(); //Configure I2C transactions
    
    enable_Interrupts(); //Globally enable interrupts
    
    configureSensors(); //Configure sensors on startup.
    
    enable_Timer(READ_TIMER); //Enable the sensor read timer for sensor data acqusition
    
    while (1) //Enter the embedded loop
    {
        bg_process_packetizer(PACKET_UART_CH_1);
        
        if (read)
            readSensors();
    }
    
}