#          Clan: RoboSub
#         Mates: Connor Henning
#  Date Created: 2015-2-7
# Last Modified: 2015-2-7
#        Scheme: Hydrophone Assembly Code
#      Ambition: Detect time delays from three hydrophones in response to a signal
#                using analog comparators and timers. Than send the information
#                over UART to the computer.



# Include Files ****************************************************************

.INCLUDE "../comparator.s"
.INCLUDE "../timer.s"
.INCLUDE "../uart.s"


# Global Symbols ***************************************************************

.GLOBAL main


# Data Segment *****************************************************************
                                                                                                                      
.DATA


# Code Segment *****************************************************************

.TEXT

# ******************************************************************************
#      Function: main
#    Programmer: Connor Henning
#  Date Created: 2015-7-15
# Last Modified: 2015-7-15
#     Objective: Main event loop, Call setup routines
#
# Modifications:
#
# Known Problems:
#
#        Inputs:
#       Outputs:

.ENT main
main:

    DI  # Call setup routines

    jal setup_UART1
    jal cmpr_setup
    jal tmr23_setup

    EI

	main_loop: # Main loop

		J main_loop
           
.END main
