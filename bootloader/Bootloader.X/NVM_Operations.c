#include "Bootloader.h"

#define NVMemIsError()    (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK))
const UINT countPerMicroSec = ((SYS_FREQ/1000000)/2);

void erase_Flash()
{
    for (int i = 0; i < (APP_MAX_ADDRESS - APP_BASE_ADDRESS)/PAGE_SIZE; i++)
    {
        NVM_Erase_Page(APP_BASE_ADDRESS + i*PAGE_SIZE);
    }
}

//Functions created with help from AN1388 Bootloader code from Microchip
void delay_us(uint32_t us)
{

    UINT targetCount;  
    UINT bakupCount; 
    // backup current count of the core timer.
    bakupCount = ReadCoreTimer();
    // Core timer increments every 2 sys clock cycles.
    // Calculate the counts required to complete "us". 
    targetCount = countPerMicroSec * us;      
    // Restart core timer.
    WriteCoreTimer(0);    
    // Wait till core timer completes the count.    
    while(ReadCoreTimer() < targetCount);
    
    // Restore count back.
    WriteCoreTimer(bakupCount + targetCount);      
}

uint32_t NVM_Write_Word(uint32_t *address, uint32_t data)
{
    uint32_t res;
    NVMADDR = KVA_TO_PA(address);
    NVMDATA = data;
    
    res = NVMOperation(NVM_WRITE_WORD);
    
    return res;
}

uint32_t NVM_Erase_Page(uint32_t *address)
{
    NVMADDR = KVA_TO_PA(address);
    
    return NVMOperation(NVM_ERASE_PAGE);
}

uint32_t NVMOperation(uint32_t nvmop)
{
    NVMCON = NVMCON_WREN | nvmop;
    delay_us(7);

    NVMKEY 		= 0xAA996655;
    NVMKEY 		= 0x556699AA;
    NVMCONSET 	= NVMCON_WR;

    // Wait for WR bit to clear
    while(NVMCON & NVMCON_WR);
    
    // Disable Flash Write/Erase operations
    NVMCONCLR = NVMCON_WREN;  
    
    return NVMemIsError();
}