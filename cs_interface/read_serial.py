#!/usr/bin/python

import serial
import signal
import sys
import os

s = None

def exit_handler(signum, frame):
	global s
	s.close()
	
def convert(char1, char2):
	bits = 16
	char1 = ord(char1)
	char2 = ord(char2)
	char1 = char1 << 8
	char1 = char1 | char2
	if (char1 & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
		char1 = char1 - (1 << bits)        # compute negative value
	return char1

def main():
	global s
	s = serial.Serial()
	s.port = 7
	s.baudrate = 9600
	s.open()


	while(True):
		byte = ord(s.read(1))
		if (byte == 0xA):
			os.system('cls')
			s.read(1)
			x1 = s.read(1)
			x2 = s.read(1)
			print convert(x2, x1)
			y1 = s.read(1)
			y2 = s.read(1)
			print convert(y2, y1)
			z1 = s.read(1)
			z2 = s.read(1)
			print convert(z2, z1)
			
			#print "\n"
			# receive the packet
			
			

if __name__ == "__main__":
	signal.signal(signal.SIGINT, exit_handler)
	main()