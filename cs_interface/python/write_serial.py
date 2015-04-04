#!/usr/bin/python

import serial
import signal
import sys
import os

s = None

def exit_handler(signum, frame):
	global s
	print "exiting"	
	s.close()
	
def main(args, num_args):
	global s

	if num_args < 2:
		port = '/dev/pts/3'
	else:
		port = args[1]
	
	print 'opening port:', port
	s = serial.Serial()
	s.port = port 
	s.baudrate = 9600
	s.open()

	while(True):
		data = raw_input()
		data += '\n'
		s.write(data)

if __name__ == "__main__":
	signal.signal(signal.SIGINT, exit_handler)
	num_args = len(sys.argv)
	args = sys.argv
	main(args, num_args)

