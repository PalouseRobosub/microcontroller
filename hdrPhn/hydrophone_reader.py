#!/usr/bin/python


		

import serial
import os
import time
from time import sleep
import signal
import sys
from math import pow
from math import atan2
from math import pi
from math import sqrt
import argparse

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
start_byte = chr(2)
stop_byte  = chr(3)

#Function Definitions#########################################################################


"""
exit handler
"""
def exit_handler(signum, frame):
	print "\nexiting!"
	s.close()
	sys.exit()



"""
	Here we are trying to make sure we have actually found
	a control byte, so we receive several packets, then look
	at where we expect the control bytes to be. If they are not in the expected
	locations, we wait for a new control byte and try again.
	X000X000X
	012345678
"""
def get_lock():
	#variables for the sync loop
	current_byte = '\0'
	packet_array = ""
	in_sync = False

	#reset the serial port
	s.close()
	s.open()

	print "Acquiring stream sync"

	while in_sync == False:
		#read a packet from the serial port
		current_byte = s.read()

		#if the byte is the control_byte, then receive several packets
		#otherwise, we will jump back to the top of the loop and get another byte
		if current_byte == start_byte:
			packet_array = "" # clear out the array
			packet_array += current_byte  # add the byte to the array

			#receive several packets
			while len(packet_array) != 8:
				packet_array += s.read()
			
			#x000000x000000x000000
			#check to see if the control byte is in the proper location in the received packets
			if  packet_array[0] == start_byte and \
				packet_array[7] == stop_byte:

				#throw away rest of last packet

				#say we are in sync so we can break out of the loop
				in_sync = True
				print "sync locked"

#end get_lock()


"""
	This function reads a 4-byte packet from the serial port.
	It will also check to make sure we are still in sync, and
	pauses the program if we lose sync. It will then attempt
	to get back into sync with the serial stream.
"""

def get_packet():

	success = False

	while success == False:

		#read 4 bytes from the serial port
		packet = s.read(8)

		#ensure we are in sync by checking that the control byte is in the correct place
		if packet[0] != start_byte and \
		   packet[7] != stop_byte: #if we are not in sync
			print "Error: lost sync. Press the [Enter] key to attempt to re-sync"
			s.flushInput() #flushes the serial rx buffer
			get_lock() #get back into sync
		else : #if we are in sync, break out of loop
			success = True


	return packet
#end get_packet()


def main(args):
	#initialize the serial port
	#s.port = args.port #this may change, depending on what port the OS gives the microcontroller
	s.port = args.port
	s.baudrate = args.baudrate		#the baudrate may change in the future
	s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)

	fileobject = open(args.file, 'w')

	#setup exit signal handlers	
	signal.signal(signal.SIGINT, exit_handler)
	signal.signal(signal.SIGTERM, exit_handler)
	
	#speed of sound in water (meters/second)
	Cs = 1250.00
	#x distance of hydrophones (meters)
	px = 0.33
	py = 0.146
	
	#distance between the hydrophones
	
	get_lock() #get in sync with the stream
	counter = 0
	while(1):
		packet = get_packet() #get a new packet from the serial interface (this is a blocking call)
		#for x in packet:
			#print hex(ord(x))
		t_h0 = ord(packet[2]) | (ord(packet[1]) << 8)
		t_hx = ord(packet[6]) | (ord(packet[5]) << 8)
		t_hy = ord(packet[4]) | (ord(packet[3]) << 8)
	
		print "{} {} {}" .format(t_h0, t_hx, t_hy)
	
		#
		tx = t_h0 - t_hx
		ty = t_h0 - t_hy
		
		
		txSec = tx * Cs / 15045000
		tySec = ty * Cs / 15045000
		
		print "tx: {}, txSec: {}".format(tx, txSec)
		print "ty: {}, tySec: {}\n".format(ty, tySec)
	
		fileobject.write("{},{},{}\n".format(t_h0, t_hx, t_hy))	
		
		#converting time to distance
		dx = txSec
		dy = tySec
		
		#algorithm
		xa = pow((dx/px), 2)
		ya = pow((dy/py), 2)
		
		xb = dx/pow(px,2) * (pow(px,2) - pow(dx,2))
		yb = dy/pow(py,2) * (pow(py,2) - pow(dy,2))
		
		xc = pow( ((pow(dx,2) - pow(px,2)) / (2 * px)), 2)
		yc = pow( ((pow(dy,2) - pow(py,2)) / (2 * py)), 2)
		
		#ra = xa + ya - 1
		#rb = xb + yb
		#rc = xc + yc
		
		ra = xa + ya - 1
		rb = xb + yb
		rc = xc + yc #pow(py, 2)
	
		output = pow(rb,2) - 4*ra*rc;
		print "output = {}, ra = {}, rb = {}, rc = {}\n".format(output, ra, rb, rc)	
		
		try:
			r1 = ( -rb + sqrt(pow(rb,2) - 4*ra*rc) ) / (2*ra)
			r2 = ( -rb - sqrt(pow(rb,2) - 4*ra*rc) ) / (2*ra)
		except:
			r1 = r2 = -rb/(2*ra)
			
		
		rx1 = (-pow(dx, 2) + 2*r1*dx + pow(px,2)) / (2*px)
		rx2 = (-pow(dx, 2) + 2*r2*dx + pow(px,2)) / (2*px)
		
		ry1 = (-pow(dy, 2) + 2*r1*dy + pow(py,2)) / (2*py)
		ry2 = (-pow(dy, 2) + 2*r2*dy + pow(py,2)) / (2*py)
		
		
		theta1 = 180/pi * atan2(ry1,rx1)
		theta2 = 180/pi * atan2(ry2,rx2)
		
		counter = counter + 1
		
		print "rx1: = {}, ry1: = {}, r1: = {}, theta1: = {}".format(rx1, ry1, r1, theta1)
		print "rx2: = {}, ry2: = {}, r2: = {}, theta2: = {}".format(rx2, ry2, r2, theta2)
		
		'''raw_input()
		s.flushInput() #flushes the serial rx buffer'''
		
		'''os.system("cls")
		print "theta 1: %f" % (theta1)
		print "theta 2: %f" % (theta2)
		print "r1: %f" % (r1)
		print "r2: %f" % (r2)
		print t_h0
		print t_hx
		print t_hy
		print counter
		sleep(.1)'''
		
	
	
if __name__ == '__main__':

	parser = argparse.ArgumentParser()
	parser.add_argument("-p", "--port", type=str, help="Serial port", default='/dev/ttyUSB0')
	parser.add_argument('-b', '--baudrate', type=int, default=9600, help="Serial interface baudrate.")
	parser.add_argument('-f', '--file', type=str, default='/dev/null')
	args = parser.parse_args()
	
	s = serial.Serial()	   #get instance of serial class
	main(args)
