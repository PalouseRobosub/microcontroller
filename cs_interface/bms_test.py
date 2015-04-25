#!/usr/bin/python
from packetizer import Packetizer
import time
import os

def main():
        p = Packetizer('/dev/ttyUSB3')
        packet [3]
	p.getlock()
        while (1):
		os.system('clear')	
		packet = p.get_packet();
		print "ID:" + packet[0]
		# Now we must convert the packet into meaningful information
		number = 0#convert the data into a value########################3 HERE##############################3		
		current = number*(3.3/1024*26.2/6.2)/1027*0.00025*0.8998
		print "Current: " + packet[1] + " Upper: " + packet[2]




if __name__ == "__main__":
        main()

#from python.packetizer import Packetizer
#import os
#import signal
#import serial
#
#
#def convert(char1, char2):
#                bits = 16
#                char1 = char1 << 8
#                char1 = char1 | char2
#                if (char1 & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
#                        char1 = char1 - (1 << bits)                # compute negative value
#                return char1
#
#	# Main program
#def main():
#	packet = []
#	p = Packetizer('/dev/ttyUSB3')	
#	p.get_lock()
#	while (1):
#		os.system('clear')
#		packet = p.receive_pakcet()
#		value = convert(packet[1], packet[2])
#		current = value*3.3/1024*26.2/6.2/1027/.00025*0.8998
#		print "ID: " + packet[0] + " Value: " + current
#
#
## Junk required to call main first
#if __name__ == "__main__":
#                main()

