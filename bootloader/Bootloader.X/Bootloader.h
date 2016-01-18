#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>

#define FRAME_LENGTH 500
#define SYS_FREQ 30000000
#define APP_BASE_ADDRESS
#define APP_MAX_ADDRESS

#define SOT 01
#define DLE 16
#define EOT 04

#define NVM_WRITE_WORD          0x4001      // Word program operation
#define NVM_ERASE_PAGE        0x4004      // Page erase operation

//Definitions
typedef enum {
    ERASE_FLASH,
    PROGRAM,
    VERIFY,
    START_EXEC
} Command;

typedef enum {
    REQ_COMMAND = 0x10,
    REQ_FRAME = 0x11,
    REQ_LAST_FRAME = 0x12,
    REQ_ADDRESS = 0x13
} Request;

typedef enum {
    DATA_RECORD,
    EOF_RECORD,
    EXT_SEG_ADDR_RECORD,
    START_SEG_ADDR_RECORD,
    EXT_LIN_ADDR_RECORD,
    START_LIN_ADDR_RECORD,
} Record_Type;

typedef struct HEX_DATA {
	UINT8 RecDataLen;
	DWORD_VAL Address;
	UINT8 RecType;
	UINT8* Data;
	UINT8 CheckSum;	
	DWORD_VAL ExtSegAddress;
	DWORD_VAL ExtLinAddress;
} Hex_Record;

//Structures used
typedef struct FRAME {
    uint8_t buffer[FRAME_LENGTH];
    uint8_t rawData[FRAME_LENGTH];
    uint8_t rawLength;
    uint8_t length;
    char isValid;
} Frame;

//Transportation Layer Functions
void initialize_UART_Interface(int baudRate, int pb_clk);
void send_Byte(uint8_t byte);
uint8_t get_Byte();
void UART_Tasks();

//Framework functions
void Framework_Tasks();
void constructRXFrame(char data);
void constructTXFrame();
void Framework_Tasks();
void handleCommand();
void HexRecord_to_Flash(uint8_t *record, int size);

//NVM Operations
uint32_t NVM_Write_Word(uint32_t *address, uint32_t data);
void delay_us(uint32_t us);
uint32_t NVMOperation(uint32_t nvmop);

//Command functions
void erase_Flash();
void begin_Execution();

#endif