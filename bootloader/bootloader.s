

# ###############################################
# This is the assembly file for the bootloader  #
# ###############################################
# 
# The bootloader utilizes pages 1 and 2 of program memory and will program all images onto pages 3+
# Page and Row size of PIC32MX1XX/2XX chips:
#       Page: 1024 Bytes
#       Row: 128 Bytes

.GLOBAL main # specify that the global entry point is function bootloader

.DATA
PROG_START_ADDR_VIRTUAL: .WORD 0x9D000800 # This is the program start address. This will be wiped if a bootloader is activated and then overwritten from there
PROG_START_ADDR_PHYSICAL: .WORD 0x1D000800 
PROG_DATA: .PAGE 0

.TEXT # Specify the text section of memory

.ENT main
main:
    DI

    # Configure UART1 to be used for receiving information
    LI $t0, 1<<11 | 1<<9
    SW $t0, U1STA       # Store the Uart status register
    LI $t0, 7
    SW $t0, U1BRG       # Configure UART baud rate
    LI $t0, 1 << 15
    SW $t0, U1MODE      # enable the UART

    # Next, configure the TRISTATE, PPS, and ANALOGSELECT for the UART
    LI $t0, 1 << 13
    SW $t0, TRISB   # This will place a 0 in the RB15 for output and 1 in RPB13 for input 

    SW $zero, ANSELB# Disable all analog selection pins 
    LI $t0, 3
    SW $t0, U1RXR   # Set the UART1 RX pin to be RPB13
    LI $t0, 1
    SW $t0, RPB15R  # Set the UART1 TX pin to be RPB15
    
    # Use UART1 to query for configuration file
    LI $t0, 0x2102424C  # Load the packet: !, 2, B, L
    SW $t0, U1TXREG     # transmit packet

    LI $t0, 6000000 # load 6M into a timeout register, for a roughly 500ms timeout with 4 instructions

    wait_response_loop:
        # Check to see UART status response, branch if response
        ADDI $t0, -1                        # Subtract one from loop index
        LW $t2, U1STA                       # Load the status of the UART
        ANDI $t2, $t2, 1                    # Query to see if receive data is available
        BNE $t2, $zero, response_received   # if we received data, we need to move to a programming state
        BNE $t0, $zero, wait_response_loop  # if we did not receive data, continue waiting until timeout complete
    
    # time out if nothing is provided to the bootloader
    # begin normal execution of the program flash memory where the bootloader would normally store the program
    J normal_execution

    # else we have received data for configuration and we should start programming memory
    response_received:

    # T3 will contain the index of the page that we are currently writing
    LW $t3, PROG_START_ADDR_PHYSICAL
    
    page_rewrite:
        # ensure that our current position is not beyond the end of programmable flash
        LI $t0, 0x1D01FFFF
        SUB $t0, $t3, $t0 # 0x1D01 FFFF is end of PFM
        BGTZ $t0, normal_execution # we can no longer program flash memory. We are out of space!

        # if we are not past the end, load the current page address into NVMADDR for erasing
        SW $t3, NVMADDR
        LI $t0, 1<<14 | 1<<2 # WREN and Page Erase operation
        JAL nvm_operation
        # now, we will write our page of memory from flash
        LI $t0, 1<<14 | 
    
    # Begin by erasing all memory that is not the bootloader and begin writing in this section

    # Now, send a response on the UART to signify that we are ready to receive
    


    # jump to normal program execution once we have written memory
    normal_execution:
    LA $t0, PROG_START_ADDR_VIRTUAL
    JR $t0

    nvm_operation: # before this is called, $t0 should hold the correct operation and WREN, ADDR should also be set
        SW $t0, NVMCON # set the WREN bit and operation

        # TIME SENSITIVE CODE!!!
        LI $t0, 0xAA996655 # WR key 1
        LI $t1, 0x556699AA # WR key 2
        LI $t2, 0x00008000 # WR bit mast
        SW $t0, NVMKEY # begin unlock sequence for WR bit
        SW $t1, NVMKEY # finish unlock sequence of WR bit
        SW $t2, NVMCONSET # Execute the specified operation
        wait_nvm_op:
            # wait until the WR bit is clear to continue
            LW $t0, NVMCON
            AND $t0, $t0, $t2
            BNE $zero, $t0, wait_nvm_op
        # the operation is complete. Clear WREN
        LI $t0, 1<<14
        SW $t0, NVMCONCLR
        JR $ra
.END main
