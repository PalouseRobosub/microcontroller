#          Clan: RoboSub
#         Mates: Connor Henning
#  Date Created: 2015-2-7
# Last Modified: 2015-2-7
#        Scheme: Hydrophone Assembly Code
#      Ambition: Detect time delays from three hydrophones in response to a signal
#                using analog comparators and timers. Than send the information
#                over UART to the computer.

#          File: UART
#      Contents: UART setup

.TEXT

# ******************************************************************************
#      Function: UART setup
#    Programmer: Connor Henning
#  Date Created: 2015-1-15
# Last Modified: 2015-7-15
#     Objective: Setup the UART transmitter with Baud rate of 9600
#
# Modifications: Baud Rate: 9600
#
# Known Problems:
#
#        Inputs:
#       Outputs:

.ENT setup_UART1
setup_UART1:

LI $t0, 15000000/(16*9600)-1
SW $t0, U1BRG

LI $t0, 0 | (1<<10)
SW $t0, U1STA

LI $t0, 1
SW $t0, RPB15R

LI $t0, 0 | (0<<1) | (1<<15)
SW $t0, U1MODE

JR $ra

.END setup_UART1
