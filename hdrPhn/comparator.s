#          Clan: RoboSub
#         Mates: Connor Henning
#  Date Created: 2015-2-7
# Last Modified: 2015-2-7
#        Scheme: Hydrophone Assembly Code
#      Ambition: Detect time delays from three hydrophones in response to a signal
#                using analog comparators and timers. Than send the information
#                over UART to the computer.

#          File: Comparator
#      Contents: Comparator Setup, Comparator ISR Handlers, and UART Packet Transmitter

.TEXT

# ******************************************************************************
#      Function: Comparator Setup
#    Programmer: Connor Henning
#  Date Created: 2015-1-15
# Last Modified: 2015-7-15
#     Objective: Setup the comparators and comparator voltage reference for the
#                hydrophones using comparator B pins.
#
# Modifications: Trigger Rising Signal
#
# Known Problems:
# 
#        Inputs:
#       Outputs: 



.ENT cmpr_setup
cmpr_setup:

LI $t0, 0 | 0xA | (0 << 4) | (0 << 5) | (1 << 6)
SW $t0, CVRCON

LI $t0, 0 | 0b00 | (1 << 4) | (0b01 << 6)
SW $t0, CM1CON
SW $t0, CM2CON
SW $t0, CM3CON

LI $t0, 0 | (1<<0) | (1<<2) | (1<<14)
SW $t0, TRISBSET


LI $t0, (0b111<<26)
SW $t0, IPC6SET

LI $t0, (0b111<<2) | (0b111<<10)
SW $t0, IPC7SET

LI $t0, 0 | (1<<0) | (1<<1) | (1<<2)
SW $t0, IFS1CLR
SW $t0, IEC1SET

# ###############################
# Modify to switch polarity of comparator
# Set for low to high signal
# Comment for high to low signal
LI $t0, (1 << 13)
SW $t0, CM1CONSET
SW $t0, CM2CONSET
SW $t0, CM3CONSET

# Modify to adjust the reference voltage
LI $t0, 0xA
SW $t0, CVRCONSET
# ##############################

LI $t0, (1<<15)

SW $t0, CVRCONSET
SW $t0, CM1CONSET
SW $t0, CM2CONSET
SW $t0, CM3CONSET

JR $ra

.END cmpr_setup


# ******************************************************************************
#      Function: ISR Comparator Handlers
#    Programmer: Connor Henning
#  Date Created: 2015-7-15
# Last Modified: 2015-7-15
#     Objective: Start a timer, if not already running when a comparator interrupt
#                is triggered and take a time stamp of the TMR register. When all
#                comparators have been triggered send the time stamps via UART.
# 
# Modifications:
# 
# Known Problems:
# 

.SECTION .vector_27, code

    J cmpr1_hndl

.TEXT

.ENT cmpr1_hndl
cmpr1_hndl:

    DI

    SW $s7, ($s6)

    LW $s0, ($s4)

    SW $t7, ($s5)

    ADDI $s3, $s3, -1

    BNE $s3, $zero, endcmp1

    JAL send_stamps

    endcmp1:

    EI

    ERET

.END cmpr1_hndl

.SECTION .vector_28, code

    J cmpr2_hndl

.TEXT

.ENT cmpr2_hndl
cmpr2_hndl:

    DI
    SW $s7, ($s6)

    LW $s1, ($s4)

    SW $t8, ($s5)

    ADDI $s3, $s3, -1

    BNE $s3, $zero, endcmp2

    JAL send_stamps

    endcmp2:

    EI

    ERET

.END cmpr2_hndl

.SECTION .vector_29, code

    J cmpr3_hndl

.TEXT

.ENT cmpr3_hndl
cmpr3_hndl:

    DI
    SW $s7, ($s6)

    LW $s2, ($s4)

    SW $t9, ($s5)

    ADDI $s3, $s3, -1

    BNE $s3, $zero, endcmp3

    JAL send_stamps

    endcmp3:

    EI

    ERET

.END cmpr3_hndl

# ******************************************************************************
#      Function: Send Time Stamps
#    Programmer: Connor Henning
#  Date Created: 2015-7-15
# Last Modified: 2015-7-15
#     Objective: Send the time stamps from the comparators over UART starting with
#                'STX' start of heading and ending with 'ETX' control bytes.
#
# Modifications:
#
# Known Problems:
# 
#        Inputs: Time Stamps
#       Outputs: 

.ENT send_stamps
send_stamps:

    LA $t1, U1TXREG
    LI $t0, 2 # STX
    SW $t0, ($t1)
    SRL $t0, $s0, 8
    SW $t0, ($t1)
    SW $s0, ($t1)
    SRL $t0, $s1, 8
    SW $t0, ($t1)
    SW $s1, ($t1)
    SRL $t0, $s2, 8
    SW $t0, ($t1)
    SW $s2, ($t1)
    LI $t0, 3 # ETX
    SW $t0, ($t1)

    JR $ra

.END send_stamps
