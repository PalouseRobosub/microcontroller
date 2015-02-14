#          Clan: RoboSub
#         Mates: Connor Henning
#  Date Created: 2015-2-7
# Last Modified: 2015-2-7
#        Scheme: Hydrophone Assembly Code
#      Ambition: Detect time delays from three hydrophones in response to a signal
#                using analog comparators and timers. Than send the information
#                over UART to the computer.

#          File: Timer
#      Contents: Timer Setup, Timer ISR Handler

.TEXT

# ******************************************************************************
#      Function: Timer 2 and 3 32 bit Setup
#    Programmer: Connor Henning
#  Date Created: 2015-1-15
# Last Modified: 2015-7-15
#     Objective: Setup timer 2 and 3 as 32 bit timers with prescaling of 1 and interrupts
#                with priority of 6
#
# Modifications: Trigger Rising Signal
#
# Known Problems:
#
#        Inputs:
#       Outputs:

.ENT tmr23_setup
tmr23_setup:

# Setup Timer Control set for 32 bit timer
LI $t0, 0 | (0 << 1) | (1 << 3) | (0b000 << 4)
SW $t0, T2CON

SW $zero, TMR2
SW $zero, TMR3
# Intial Clear of time stamps registers and interrupt counter
MOVE $s0, $zero
MOVE $s1, $zero
MOVE $s2, $zero
LI $s3, 3
LI $t0, (6<<2)
SW $t0, IPC3



# Setup Period Registers
LI $t0, 0xFFFF
SW $t0, PR2
LI $t0, 0x8F
SW $t0, PR3

# Configure Interrupt
LI $t0, (1<<14)
SW $t0, IFS0CLR
SW $t0, IEC0SET

JR $ra

.END tmr23_setup

# ******************************************************************************
#      Function: Timer 2 and 3 32 bit ISR Handler
#    Programmer: Connor Henning
#  Date Created: 2015-1-15
# Last Modified: 2015-7-15
#     Objective: Reset
#
# Modifications: Trigger Rising Signal
#
# Known Problems:
#

.SECTION .vector_12, code
    J tmr23_hndl

.TEXT
.ENT tmr23_hndl
tmr23_hndl:

    DI

    LI $t0, (1 << 15)
    SW $t0, T2CONCLR

    SW $zero, TMR2
    SW $zero, TMR3

    # Reset timestamp registers
    MOVE $s0, $zero
    MOVE $s1, $zero
    MOVE $s2, $zero
    LI $s3, 3

    LI $t0, 0 | (1<<0) | (1<<1) | (1<<2)
    SW $t0, IEC1SET
    SW $t0, IFS1CLR

    LI $t0, (1<<14)
    SW $t0, IFS0CLR

    EI

    ERET

.END tmr23_hndl



# .ENT timer1_setup
# timer1_setup:
#
#     LI $t0, 0 | (0 << 1) | (0b00 << 4)
#     SW $t0, T1CON
#
#     LI $t0, 65535
#     SW $t0, PR1
#
#     SW $zero, TMR1
#
#     LI $t0, (6 << 2)
#     SW $t0, IPC1SET
#     LI $t0, (1 << 4)
#     SW $t0, IFS0CLR
#     SW $t0, IFS0SET
#
#     JR $ra
#
# .END timer1_setup
#
# .SECTION .vector_4, code
#
#     J timer1_handler
#
# .TEXT
#
# .ENT timer1_handler
# timer1_handler:
#
#     DI
#
#     LI $t0, (1 << 4)
#     SW $t0, IFS0CLR
#
#
#     SW $s0, U1TXREG
#     SW $s1, U1TXREG
#     SW $s2, U1TXREG
#
#
#
#     LI $t0, 0 | (1<<0) | (1<<1) | (1<<2)
#     SW $t0, IFS1CLR
#     SW $t0, IEC1SET
#
#     EI
#
#     ERET
# .END timer1_handler
