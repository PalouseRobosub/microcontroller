

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
PROG_DATA_PAGE_OFFSET: .WORD 0x00000000 # page address for current programming
PROG_DATA: .PAGE 0
TIMER_TIMOUTS: .BYTE 0 # variable for counting number of timer timeouts

.TEXT # Specify the text section of memory

.ENT main
main:
    DI

    # #####################################################################
    # Timer Configuration Section                                         #
    # --------------------------------------------------------------------#
    # This configures Timer1 to be used as a timeout timer. The timeout   #
    # timer is used when receiving UART or querying the computer for data #
    # #####################################################################
    SW $zero, T1CON
    SW $zero, TMR1
    LI $t0, 23438 # Store 23438 into the period register, at 64 clock div. This gives us (15MHz/(64*23438)) = 9.9997 Hz (roughly 100ms)
    SW $t0, PR1
    LI $t0, 0b10 << 4
    SW $t0, T1CON

    # #####################################################################
    # UART Configuration Section                                          #
    # --------------------------------------------------------------------#
    # Configure UART1 to be used for receiving information                #
    # #####################################################################
    LI $t0, 1<<11 | 1<<9
    SW $t0, U1STA       # Store the Uart status register
    LI $t0, 7
    SW $t0, U1BRG       # Configure UART baud rate
    LI $t0, 1 << 15
    SW $t0, U1MODE      # enable the UART

    # #####################################################################
    # Pin Configuration Section                                           #
    # --------------------------------------------------------------------#
    # Cofngiures pin tristates as needed to support the UART              #
    # This means pin B15 must be the U1TX and pin B13 must be U1RX        #
    # #####################################################################
    # Next, configure the TRISTATE, PPS, and ANALOGSELECT for the UART
    LI $t0, 1 << 13
    SW $t0, TRISB   # This will place a 0 in the RB15 for output and 1 in RPB13 for input 

    SW $zero, ANSELB# Disable all analog selection pins 
    LI $t0, 3
    SW $t0, U1RXR   # Set the UART1 RX pin to be RPB13
    LI $t0, 1
    SW $t0, RPB15R  # Set the UART1 TX pin to be RPB15



    EI

    # #######################################################################
    # UART Query Section                                                    #
    # ------------------------------------------------------------------    #
    # Use UART1 to query for configuration file                             #
    # #######################################################################
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
    SW $t3, PROG_DATA_PAGE_OFFSET # store the start address in our programming pointer at initialization
    

    # ################################################################
    # Flash Reprogram Function                                       #
    # ---------------------------------------------------------------#
    # This function will rewrite a page of flash memory with the     #
    # data stored within PROG_DATA variable.                         #
    # ################################################################
    page_rewrite:
        LW $t3, PROG_DATA_PAGE_OFFSET # load the start address of programming location

        # ensure that our current position is not beyond the end of programmable flash
        LI $t0, 0x1D01FFFF
        SUB $t0, $t3, $t0 # 0x1D01 FFFF is end of PFM
        BGTZ $t0, normal_execution # we can no longer program flash memory. We are out of space!

        # if we are not past the end, load the current page address into NVMADDR for erasing
        SW $t3, NVMADDR
        LI $t0, 1<<14 | 1<<2 # WREN and Page Erase operation
        JAL nvm_operation

        
        # now, we will write one row of memory to flash until the page has been written

        LI $s0, 8 # This will need to run the loop through 8 rows to program a page
        LA $s1, PROG_DATA_PAGE_OFFSET # load the address of the program data
        row_program:

            LI $t0, 1<<14 | 0b11 # Set WREN and 011 (row program) as operation
            SW $s1, NVMSRCADDR # store the source address in RAM for the NVM row program
            SW $t3, NVMADDR
            JAL nvm_operation
            ADDI $s1, $s1, 128 # increment the address of program data by one row
            ADDI $t3, $t3, 128 # increment the address of EEPROM by one row
            ADDI $s0, $s0, -1  # decrement the s0 register by 1 to indicate a successful row write
            BNE $zero, $s0, row_program # check for more loops

        SW $t3, PROG_DATA_PAGE_OFFSET # now we store the t3 variable back into PROG_DATA_PAGE_OFFSET, it should be incremented by 1024
        JR $ra # jump back to calling program


    # ##################################################################
    # Begin normal execution                                           #
    # -----------------------------------------------------------------#
    # Jump to normal program memory after the bootloader has completed #
    # ##################################################################
    normal_execution:
    LA $t0, PROG_START_ADDR_VIRTUAL
    JR $t0

    # ###################################################################
    # NonVolatile Memory Operation                                      #
    # ------------------------------------------------------------------#
    # this is a function that should be supplied a nvm operation in $t0 #
    # This function will perform an operation on the nonvolatile memory #
    # Address (physical address) should be set in the NVMADDR regoster  #
    # Before this function is called                                    #
    # Use JAL to call this, as the program will jump back to $ra        #
    # ###################################################################
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
