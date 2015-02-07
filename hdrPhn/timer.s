# Timer Module
# Description: Setup timer to count time between hydrophone pings

# Function: Timer1 Setup
# Programmer: Connor Henning
# Date Created: 2015-1-20
# Last Date Modified:
# Latest Programmer:
# Description: Setup up timer1 for hydrophone input

.TEXT

.ENT timer23_setup
timer23_setup:

# Setup Timer Control set for 32 bit timer
LI $t0, 0 | (0 << 1) | (1 << 3) | (0b000 << 4)
SW $t0, T2CON

SW $zero, TMR2
SW $zero, TMR3

# Configure Interrupt
LI $t0, (1<<14)
SW $t0, IFS0CLR
SW $t0, IEC0SET

LI $t0, (6<<2)
SW $t0, IPC3


# Setup Period Registers
LI $t0, 0xF
SW $t0, PR2
LI $t0, 0xF
SW $t0, PR3

JR $ra

.END timer23_setup

.SECTION .vector_12, code
    J timer23_handler

.TEXT
.ENT timer23_handler
timer23_handler:

    DI

    LI $t0, 15
    SW $t0, T2CONCLR

    # Reset timestamp registers
    MOVE $s0, $zero
    MOVE $s1, $zero
    MOVE $s2, $zero
    LI $s3, 3

    LI $t0, 0 | (1<<0) | (1<<1) | (1<<2)
    SW $t0, IEC1SET
    SW $t0, IFS1CLR

    EI

    ERET

.END timer23_handler



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
