#include <GenericTypeDefs.h>
#include "Bootloader.h"

extern Frame RX, TX;

void Framework_tasks()
{
    //Handle RX frames if they are valid
    if (RX.isValid)
    {
        RX.isValid = 0; //clear our flag
        handleCommand();
    }
    if (TX.rawLength) //If there is raw data ready for us to create a frame of
        //Next, construct our TX frame
        constructTXFrame();
}

void handleCommand()
{
    uint8_t command = RX.buffer[0];
    
    TX.rawLength = 0;
    TX.rawData[TX.rawLength++] = command; //Prepare our TX packet
    
    switch (command)
    {
        case ERASE_FLASH:
            erase_Flash();
            break;
        case PROGRAM:
            HexRecord_to_Flash(&RX.buffer[1], RX.length - 1);
            break;
        case VERIFY:
            break;
        case START_EXEC:
            begin_Execution();
            break;
    }
    
}

void HexRecord_to_Flash(uint8_t *buffer, int size)
{
    int i;
    void *progAddress;
    uint32_t wrData;
    
    static Hex_Record hexRec;
    if (size >= 5)
    {
        hexRec.RecDataLen = buffer[0];
        hexRec.RecType = buffer[3];
        hexRec.Data = &(buffer[4]);
        
        //First, verify our checksum
        uint8_t checksum = 0;
        for (i = 0; i < hexRec.RecDataLen + 5; i++)
            checksum += buffer[i];

        if (checksum != 0)
        {
            //Error! Request a RESEND
            TX.rawData[TX.rawLength++] = REQ_LAST_FRAME;
        }
        else
        {
            //If our checksum was a success, 
            switch (hexRec.RecType)
            {
                case DATA_RECORD:
                    //If this is a data record, we must program all of the words starting at the supplied address
                    hexRec.Address.byte.MB = 0;
                    hexRec.Address.byte.UB = 0;
                    hexRec.Address.byte.HB = buffer[1];
                    hexRec.Address.byte.LB = buffer[2];
                    
                    hexRec.Address.Val = hexRec.Address.Val + hexRec.ExtLinAddress.Val + hexRec.ExtSegAddress.Val;
                    
                    //Write all bytes of data
                    while (hexRec.RecDataLen)
                    {
                        progAddress = (void *)KVA_TO_PA(hexRec.Address.Val);
                        
                        if(((progAddress >= (void *)KVA_TO_PA(APP_BASE_ADDRESS)) && 
                                (progAddress <= (void *)KVA_TO_PA(APP_MAX_ADDRESS)))
						   && 
                                ((progAddress < (void*)KVA_TO_PA(DEV_CFG_BASE_ADDRESS)) 
                                || (progAddress > (void*)KVA_TO_PA(DEV_CFG_MAX_ADDRESS))))
						{
							if(hexRec.RecDataLen < 4)
							{
								
								// Sometimes record data length will not be in multiples of 4. Appending 0xFF will make sure that..
								// we don't write junk data in such cases.
								wrData = 0xFFFFFFFF;
								memcpy(&wrData, hexRec.Data, hexRec.RecDataLen);	
							}
							else
							{	
								memcpy(&wrData, hexRec.Data, 4);
							}		
							// Write the data into flash.	
							NVM_Write_Word(progAddress, wrData);	
                        }
						// Increment the address.
						hexRec.Address.Val += 4;
						// Increment the data pointer.
						hexRec.Data += 4;
						// Decrement data len.
						if(hexRec.RecDataLen > 3)
						{
							hexRec.RecDataLen -= 4;
						}	
						else
						{
							hexRec.RecDataLen = 0;
						}	
                    }
                    
                    break;
                case EXT_SEG_ADDR_RECORD:// Record Type 02, defines 4th to 19th bits of the data address.
				    hexRec.ExtSegAddress.byte.MB = 0;
					hexRec.ExtSegAddress.byte.UB = hexRec.Data[0];
					hexRec.ExtSegAddress.byte.HB = hexRec.Data[1];
					hexRec.ExtSegAddress.byte.LB = 0;
					// Reset linear address.
					hexRec.ExtLinAddress.Val = 0;
                    break;
                    
                case EXT_LIN_ADDR_RECORD:   // Record Type 04, defines 16th to 31st bits of the data address. 
					hexRec.ExtLinAddress.byte.MB = hexRec.Data[0];
					hexRec.ExtLinAddress.byte.UB = hexRec.Data[1];
					hexRec.ExtLinAddress.byte.HB = 0;
					hexRec.ExtLinAddress.byte.LB = 0;
					// Reset segment address.
					hexRec.ExtSegAddress.Val = 0;
                    break;
                    
                case EOF_RECORD:
                default:
					hexRec.ExtSegAddress.Val = 0;
					hexRec.ExtLinAddress.Val = 0;
                    break;
            }
        }
    }
}

void constructTXFrame()
{
    int i;
    //go through raw data and place it into the buffer
    TX.buffer[0] = SOT;
    TX.length = 1;
    for (i = 0; i < TX.rawLength; i++)
    {
        if (TX.rawData[i] == SOT || TX.rawData[i] == EOT || TX.rawData[i] == DLE)
            TX.buffer[TX.length++] = DLE;
        
        TX.buffer[TX.length++] = TX.rawData[i];
    }
    TX.buffer[TX.length++] = EOT;
    TX.rawLength = 0;
    TX.isValid = 1;
}

void constructRXFrame(char byte)
{
    static char esc = 0;
    
    
    //Convert on the fly
    switch (byte)
    {
        case SOT:
            if (esc)
            {
                esc = 0;
                RX.buffer[RX.length++] = byte;
            }
            else
            {
                RX.length = 0;
            }
            break;
        case EOT:
            if (esc)
            {
                esc = 0;
                RX.buffer[RX.length++] = byte;
            }
            else
            {
                RX.isValid = 1;
            }
            break;
        case DLE:
            if (esc)
            {
                esc = 0;
                RX.buffer[RX.length++] = byte;
            }
            else
            {
                esc = 1;
            }
            break;
        default:
            RX.buffer[RX.length++] = byte;
            break;
    }
}