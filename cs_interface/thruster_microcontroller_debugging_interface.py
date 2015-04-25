#!/usr/bin/python

#import statements
import argparse
import serial
import os
import sys
import time
import signal
from microcontroller_lib import Thruster
	

"""
exit handler
"""
def exit_handler(signum, frame) :
	print "\nexiting!\n"
	
	#make sure the thrusters get shut down
	thruster_X_A.send(0)
	thruster_X_B.send(0)
	thruster_Y_A.send(0)
	thruster_Y_B.send(0)
	thruster_Z_A.send(0)
	thruster_Z_B.send(0)
	
	thruster_X_A.send(0)
	thruster_X_B.send(0)
	thruster_Y_A.send(0)
	thruster_Y_B.send(0)
	thruster_Z_A.send(0)
	thruster_Z_B.send(0)
	
	s.close()
	sys.exit()

#Main code####################################################################################
def main(args):
	if (sys.platform == "win32") : #windows
		CLEAR_CMD = "cls"
	else : 				   #some other OS (should insert more checks)
		CLEAR_CMD = "clear"
		
	
	
	#initialize the serial port
	s.port = args.port #this may change, depending on what port the OS gives the microcontroller
	s.baudrate = args.baudrate #the baudrate may change in the future
	s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)
	
	signal.signal(signal.SIGINT, exit_handler)
	signal.signal(signal.SIGTERM, exit_handler)	
	
	#initalize the thrusters
	X_HBRIDGE_ADDR = 128
	Y_HBRIDGE_ADDR = 129
	Z_HBRIDGE_ADDR = 130
	
	
	thruster_X_A = Thruster(X_HBRIDGE_ADDR, 1, s)
	thruster_X_B = Thruster(X_HBRIDGE_ADDR, 2, s)
	thruster_Y_A = Thruster(Y_HBRIDGE_ADDR, 1, s)
	thruster_Y_B = Thruster(Y_HBRIDGE_ADDR, 2, s)
	thruster_Z_A = Thruster(Z_HBRIDGE_ADDR, 1, s)
	thruster_Z_B = Thruster(Z_HBRIDGE_ADDR, 2, s)
	
	
	#clear the screen
	os.system(CLEAR_CMD)
	
	magnitude = 0
	direction = 1
	
	
	thruster_X_A.send(0)
	thruster_X_B.send(0)
	thruster_Y_A.send(0)
	thruster_Y_B.send(0)
	thruster_Z_A.send(0)
	thruster_Z_B.send(0)
	
	
	#Main reading loop
	while 1 :
	
		time.sleep(.01)
		
		thruster_X_A.send(magnitude*direction) #2
		thruster_X_B.send(magnitude*direction) #4
		thruster_Y_A.send(magnitude*direction) #1
		thruster_Y_B.send(magnitude*direction) #3
		thruster_Z_A.send(magnitude*direction) #2
		thruster_Z_B.send(magnitude*direction) #3
		
		#clear the screen
		os.system(CLEAR_CMD)
		print "Magnitude: " + str(magnitude) + '\n' \
			  "Direction: " + str(direction) + '\n'
		
		magnitude += 1
		if (magnitude > 127) :
			magnitude = 0
			direction = -1*direction	
		
	#end of reading while loop 
#end main


if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument("-p", "--port", type=str, default="/dev/ttyUSB1", help="Serial port")
	parser.add_argument('-b', '--baudrate', type=int, default=9600, help="Serial interface baudrate.")
	args = parser.parse_args()

	s = serial.Serial()    #get instance of serial class
	
	main(args)
