

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This is the assembly file for the bootloader ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.GLOBAL bootloader ; specify that the global entry point is function bootloader

.RDATA ; Read only data
U1MODE_REG:      .WORD 0x00008000 ; The UART is enabled with all other values set to 0
U1STATUS_REG:   .WORD 0x00001400 ; Enable the receiver and the transmitter 
U1BRG_REG:      .WORD 0x00000007 ; BRG of 7 for 115.200k Baud Rate


.TEXT ; Specify the text section of memory

.ENT bootloader
bootloader:
    DI

    ; Configure UART1 to be used for receiving information
    LW $t0, U1STATUS_REG
    SW $t0, U1STA       ; Store the Uart status register
    LW $t0, U1BRG_REG
    SW $t0, U1BRG       ; Configure UART baud rate
    LW $t0, U1MODE_REG
    SW $t0, U1MODE      ; enable the UART
    
    ; Use UART1 to query for configuration file
    LI $t0, 0x2102424C  ; Load the packet: !, 2, B, L
    SW $t0, U1TXREG     ; transmit packet

    LI $t0, 6000000 ; load 6M into a timeout register, for a roughly 500ms timeout with 4 instructions
    LI $t1, 1

    wait_response_loop:
        ; Check to see UART status response, branch if response
        LW $t2, U1STA                       ; Load the status of the UART
        AND $t2, $t2, $t1                   ; Query to see if receive data is available
        BNE $t2, $zero, response_received   ; if we received data, we need to move to a programming state
        BNE $t0, $zero, wait_response_loop  ; if we did not receive data, continue waiting until timeout complete

    ; time out if nothing is provided to the bootloader
    ; begin normal execution of the program flash memory where the bootloader would normally store the program


    ; else we have received data for configuration and we should start programming memory
    response_received:

    ; jump to normal program execution once we have written memory


.END bootloader