# UART module
# Description: Setup UART to send hydrophone data


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
