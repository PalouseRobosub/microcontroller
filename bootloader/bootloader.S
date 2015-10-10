#include "/home/ryan/repositories/microcontroller/lib/include/sublibinal_config.h"

;# ###############################################
;# This is the assembly file for the bootloader  #
;# ###############################################
;# 
;# The bootloader utilizes pages 1 and 2 of program memory and will program all images onto pages 3+
;# Page and Row size of PIC32MX1XX/2XX chips:
;#       Page: 1024 Bytes
;#       Row: 128 Bytes

.GLOBAL main ;# specify that the global entry point is function bootloader

.DATA
PROG_START_ADDR_VIRTUAL: .WORD 0x9D000800 ;# This is the program start address. This will be wiped if a bootloader is activated and then overwritten from there
PROG_START_ADDR_PHYSICAL: .WORD 0x1D000800 
PROG_DATA_PAGE_OFFSET: .WORD 0x00000000 ;# page address for current programming
PROG_DATA_BYTE_OFFSET: .WORD 0x00000000 ;# offset for a UART transaction
PROG_DATA: .PAGE 
TIMER_TIMEOUTS: .BYTE 0 ;# variable for counting number of timer timeouts

.TEXT ;# Specify the text section of memory

.ENT main
main:
    DI

    ;# ######################################################################
    ;# Timer Configuration Section                                         ;#
    ;# --------------------------------------------------------------------;#
    ;# This configures Timer1 to be used as a timeout timer. The timeout   ;#
    ;# timer is used when receiving UART or querying the computer for data ;#
    ;# ######################################################################
    SW $zero, T1CON
    SW $zero, TMR1
    LI $t0, 23438 ;# Store 23438 into the period register, at 64 clock div. This gives us (15MHz/(64*23438)) = 9.9997 Hz (roughly 100ms)
    SW $t0, PR1
    LI $t0, 0b10 << 4
    SW $t0, T1CON

    ;# Configure the timer interrupt
    LI $t0, 1 << 4
    SW $t0, IFS0CLR
    LI $t1, 7<<2 | 7
    SW $t1, IPC1SET ;# set the priority and sub priority
    SW $t0, IEC0SET  ;# enable the interrupt

    ;# ######################################################################
    ;# UART Configuration Section                                          ;#
    ;# --------------------------------------------------------------------;#
    ;# Configure UART1 to be used for receiving information                ;#
    ;# ######################################################################
    LI $t0, 1<<11 | 1<<9
    SW $t0, U1STA       ;# Store the Uart status register
    LI $t0, 7
    SW $t0, U1BRG       ;# Configure UART baud rate
    LI $t0, 1 << 15
    SW $t0, U1MODE      ;# enable the UART

    ;# enable receiver interrupt
    LI $t0, 1<<8
    SW $t0, IFS1CLR
    SW $t0, IEC1SET
    LI $t0, 7<<2 | 7
    SW $t0, IPC8
    

    ;# ######################################################################
    ;# Pin Configuration Section                                           ;#
    ;# --------------------------------------------------------------------;#
    ;# Cofngiures pin tristates as needed to support the UART              ;#
    ;# This means pin B15 must be the U1TX and pin B13 must be U1RX        ;#
    ;# ######################################################################
    ;# Next, configure the TRISTATE, PPS, and ANALOGSELECT for the UART
    LI $t0, 1 << 13
    SW $t0, TRISB   ;# This will place a 0 in the RB15 for output and 1 in RPB13 for input 

    SW $zero, ANSELB;# Disable all analog selection pins 
    LI $t0, 3
    SW $t0, U1RXR   ;# Set the UART1 RX pin to be RPB13
    LI $t0, 1
    SW $t0, RPB15R  ;# Set the UART1 TX pin to be RPB15



    EI

    ;# ######################################################################
    ;# UART Query Section                                                    ;#
    ;# ------------------------------------------------------------------    ;#
    ;# Use UART1 to query for configuration file                             ;#
    ;# ######################################################################
    LI $t0, 0x2102424C  ;# Load the packet: !, 2, B, L
    SW $t0, U1TXREG     ;# transmit packet
    
    ;# enable the timer
    LI $t0, 1<<15
    SW $t0, T1CONSET

    wait_response_loop:
        ;# Check to see UART status response, branch if response
        LW $t1, PROG_DATA_BYTE_OFFSET       ;# if byteoffset is nonzero, we have received
        LW $t2, TIMER_TIMEOUTS              ;# if timertimeouts is greater than 0, we have timed out
        BNE $t1, $zero, response_received   ;# if we received data, we need to move to a programming state
        BNE $t2, $zero, normal_execution    ;# if we did not receive data and have timedout, begin normal execution
        J wait_response_loop                ;# if we haven't timed out and haven't received, keep waiting



    response_received:

    ;# reset the variables
    SW $zero, PROG_DATA_BYTE_OFFSET
    SW $zero, TIMER_TIMEOUTS

    SW $zero, TMR1 ;# clear timer count
    LI $t0, 0x21024C4C ;# send: !, 2, L, L
    SW $t0, U1TXREG ;# transmit a ready package

    LW $t3, PROG_START_ADDR_PHYSICAL
    SW $t3, PROG_DATA_PAGE_OFFSET ;# store the start address in our programming pointer at initialization

    ;# now we will loop waiting for a receive
    LI $t3, 1
    LI $t2, 1024

    receive_program:
        LW $t0, TIMER_TIMEOUTS
        BEQ $t3, $t0, timeout_program ;# timeout after 300ms of no receives
        LW $t0, PROG_DATA_BYTE_OFFSET 
        BEQ $t2, $t0, page_received
    J receive_program

    timeout_program:
        ;# first, query computer to see if byte count is accurate
        ;# if byte count is accurate, jump to page_received

    page_received:

    

    ;# #################################################################
    ;# Flash Reprogram Function                                       ;#
    ;# ---------------------------------------------------------------;#
    ;# This function will rewrite a page of flash memory with the     ;#
    ;# data stored within PROG_DATA variable.                         ;#
    ;# #################################################################
    page_rewrite:
        LW $t3, PROG_DATA_PAGE_OFFSET ;# load the start address of programming location

        ;# ensure that our current position is not beyond the end of programmable flash
        LI $t0, 0x1D01FFFF
        SUB $t0, $t3, $t0 ;# 0x1D01 FFFF is end of PFM
        BGTZ $t0, normal_execution ;# we can no longer program flash memory. We are out of space!

        ;# if we are not past the end, load the current page address into NVMADDR for erasing
        SW $t3, NVMADDR
        LI $t0, 1<<14 | 1<<2 ;# WREN and Page Erase operation
        JAL nvm_operation

        
        ;# now, we will write one row of memory to flash until the page has been written

        LI $s0, 8 ;# This will need to run the loop through 8 rows to program a page
        LA $s1, PROG_DATA_PAGE_OFFSET ;# load the address of the program data
        row_program:

            LI $t0, 1<<14 | 0b11 ;# Set WREN and 011 (row program) as operation
            SW $s1, NVMSRCADDR ;# store the source address in RAM for the NVM row program
            SW $t3, NVMADDR
            JAL nvm_operation
            ADDI $s1, $s1, 128 ;# increment the address of program data by one row
            ADDI $t3, $t3, 128 ;# increment the address of EEPROM by one row
            ADDI $s0, $s0, -1  ;# decrement the s0 register by 1 to indicate a successful row write
            BNE $zero, $s0, row_program ;# check for more loops

        SW $t3, PROG_DATA_PAGE_OFFSET ;# now we store the t3 variable back into PROG_DATA_PAGE_OFFSET, it should be incremented by 1024
        JR $ra ;# jump back to calling program


    ;# ###################################################################
    ;# Begin normal execution                                           ;#
    ;# -----------------------------------------------------------------;#
    ;# Jump to normal program memory after the bootloader has completed ;#
    ;# ###################################################################
    normal_execution:
    LA $t0, PROG_START_ADDR_VIRTUAL
    JR $t0

    ;# ####################################################################
    ;# NonVolatile Memory Operation                                      ;#
    ;# ------------------------------------------------------------------;#
    ;# this is a function that should be supplied a nvm operation in $t0 ;#
    ;# This function will perform an operation on the nonvolatile memory ;#
    ;# Address (physical address) should be set in the NVMADDR regoster  ;#
    ;# Before this function is called                                    ;#
    ;# Use JAL to call this, as the program will jump back to $ra        ;#
    ;# ####################################################################
    nvm_operation: ;# before this is called, $t0 should hold the correct operation and WREN, ADDR should also be set
        SW $t0, NVMCON ;# set the WREN bit and operation

        ;# TIME SENSITIVE CODE!!!
        LI $t0, 0xAA996655 ;# WR key 1
        LI $t1, 0x556699AA ;# WR key 2
        LI $t2, 0x00008000 ;# WR bit mast
        SW $t0, NVMKEY ;# begin unlock sequence for WR bit
        SW $t1, NVMKEY ;# finish unlock sequence of WR bit
        SW $t2, NVMCONSET ;# Execute the specified operation
        wait_nvm_op:
            ;# wait until the WR bit is clear to continue
            LW $t0, NVMCON
            AND $t0, $t0, $t2
            BNE $zero, $t0, wait_nvm_op
        ;# the operation is complete. Clear WREN
        LI $t0, 1<<14
        SW $t0, NVMCONCLR
        JR $ra


.END main

;# ###############################################################
;# Timeout Interrupt on Timer1                                  ;#
;# -------------------------------------------------------------;#
;# This is the ISR for timer1. This function will increment the ;#
;# number of timeouts and then reset the timer value.           ;#
;# ###############################################################

.SECTION .vector_4, code
    J timeoutHandler
.TEXT

.ENT timeoutHandler
timeoutHandler:
    DI 
    
    ;# Save variable $t5
    SW $t5, -4($sp)

    LW $t5, TIMER_TIMEOUTS
    ADDI $t5, $t5, 1
    SW $t5, TIMER_TIMEOUTS
    Sw $zero, TMR1 ;# reset the timer value

    ;# clear timer1 flag
    LI $t5, 1<<4
    SW $t5, IFS0CLR 
    
    ;# restore t5
    LW $t5, -4($sp)

    EI
    ERET
.END timeoutHandler

;# ###################################################################
;# UART Receive Handler                                             ;#
;# -----------------------------------------------------------------;#
;# This function receives data on UART and stores the byte into the ;#
;# program_data variable to be programmed into the flash memory     ;#
;# ###################################################################

.SECTION .vector_32, code
    J receiveHandler
.TEXT

.ENT receiveHandler
receiveHandler:
    DI
    SW $t5, -4($sp)
    SW $t4, -8($sp)

    ;# load program memory and byte offset
    LW $t4, PROG_DATA_BYTE_OFFSET
    LA $t5, PROG_DATA
    ADD $t5, $t5, $t4 ;# add them together to get our memory address in prog data

    

    ;# store the received byte into the RAM
    LW $t4, U1RXR
    SB $t4, ($t5)

    ;# increment byte offset and store back in memory
    ADDI $t4, $t4, 1
    SW $t4, PROG_DATA_BYTE_OFFSET

    ;# Reset the timer count
    SW $zero, TMR1
    SW $zero, TIMER_TIMEOUTS

    ;# restore context and continue normal execution
    LW $t4, -8($sp)
    LW $t5, -4($sp)
    EI
    ERET

.END receiveHandler
