# Comparator Module
# Description: Setup analog compartor modules and comparator ISRs

# Function: Comparator Setup
# Programmer: Connor Henning
# Date Created: 2015-1-17
# Last Date Modified:
# Latest Programmer:
# Description: Setup up analog comparator for hydrophone input

.TEXT

.ENT comparator_setup
comparator_setup:

LI $t0, 0 | 0xA | (0 << 4) | (0 << 5) | (0 << 6)
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
SW $t0, CM2CONSET

# Modify to adjust the reference voltage
LI $t0, 0xA
SW $t0, CVRCONSET
# ###############################

LI $t0, (1<<15)

SW $t0, CVRCONSET
SW $t0, CM1CONSET
SW $t0, CM2CONSET
SW $t0, CM3CONSET



JR $ra

.END comparator_setup

.SECTION .vector_27, code

    J cmp1_handler

.TEXT

.ENT cmp1_handler
cmp1_handler:

    DI

    LI $t0, (1 << 15)
    SW $t0, T2CONSET

    LW $s0, TMR2

    LI $t0, (1 << 0)
    SW $t0, IEC1CLR

    SUB $s3, $s3, 1

    BNE $s3, $zero, endcmp1

    JAL send_stamps

    endcmp1:

    EI

    ERET

.END cmp1_handler

.SECTION .vector_28, code

    J cmp2_handler

.TEXT

.ENT cmp2_handler
cmp2_handler:

    DI
    LI $t0, (1 << 15)
    SW $t0, T2CONSET

    LW $s1, TMR2

    LI $t0, (1 << 1)
    SW $t0, IEC1CLR

    SUB $s3, $s3, 1

    BNE $s3, $zero, endcmp2

    JAL send_stamps

    endcmp2:

    EI

    ERET

.END cmp2_handler

.SECTION .vector_29, code

    J cmp3_handler

.TEXT

.ENT cmp3_handler
cmp3_handler:

    DI
    LI $t0, (1 << 15)
    SW $t0, T2CONSET

    LW $s2, TMR2

    LI $t0, (1 << 2)
    SW $t0, IEC1CLR

    SUB $s3, $s3, 1

    BNE $s3, $zero, endcmp3

    JAL send_stamps

    endcmp3:

    EI

    ERET

.END cmp3_handler

.ENT send_stamps
send_stamps:

    LA $t1, U1TXREG
    LI $t0, '\n'
    SW $t0, ($t1)
    SRL $t0, $s0, 8
    SW $t0, ($t1)
    SW $s0, ($t1)
    SRL $t0, $t1, 8
    SW $t0, ($t1)
    SW $s1, ($t1)
    SRL $t0, $t2, 8
    SW $t0, ($t1)
    SW $s2, ($t1)

    JR $ra

.END send_stamps
