
#include "Configuration.h" //Device configuration of oscillators and other peripherals
#include "Bootloader.h" //Main bootloader header

char runApp = 0;
Frame RX, TX;

int main()
{
    TX.rawLength = 0;
    RX.rawLength = 0;
    RX.isValid = 0;
    TX.isValid = 0;
    
    initialize_UART_Interface(115200, PB_CLK);
    
    while (1)
    {
        if (runApp)
            begin_Execution();
        
        UART_tasks(); //Receive any data in the RX buffer and transmit any TX data
        Framework_tasks(); //Process any frames we have received
    }
}

void begin_Execution()
{
    void (*programStart) (void);
	programStart = (void (*)(void))(APP_BASE_ADDRESS);
    programStart();
}