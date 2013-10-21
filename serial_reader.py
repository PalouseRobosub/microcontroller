#!/usr/bin/python

#import statements
import serial
import os
import time

#global constants
control_byte = '\n'
ACL_1_X_addr = ord('X')
ACL_1_Y_addr = ord('Y')
ACL_1_Z_addr = ord('Z')
GYRO_1_X_addr = ord('I')
GYRO_1_Y_addr = ord('J')
GYRO_1_Z_addr = ord('K')


#clear the screen
os.system('cls')

#initialize the serial port
s = serial.Serial()
s.port = 2
s.baudrate = 56818
s.open()

start_time = time.time()
packet_count = 0



"""
	Here we are trying to make sure we have actually found
	a control byte, so we receive several packets, then look
	at where we expect the control bytes to be. If they are not in the expected
	locastions, we wait for a new control byte and try again.

	X000X000X
	012345678
"""
def get_lock() :
	#variables for the sync loop
	mchar = '\0'
	mstring = ""
	in_sync = False
	while in_sync == False:
#		print "waiting for control byte"
		mchar = s.read()

		if mchar == control_byte :
#			print "found control byte"
			mstring = ""
			mstring += mchar
			
#			print "reading several packets"
			#receive several packets
			while len(mstring) != 9 :
				mstring += s.read()
#				print "%d bytes left" % (9-len(mstring))
			
#			print "all bytes received. values:"
#			print mstring

			if (mstring[0] == control_byte and \
				mstring[4] == control_byte and \
				mstring[8] == control_byte) : #we have sync
				
#				print "control byte found!"
				#throw away rest of last packet
				s.read()
				s.read()
				s.read()

				#say we are in sync so we can break out
				in_sync = True
				print "sync locked"

			#don't need an else case, we just start loop again

def get_packet() :

	success = False
	
	while success == False :
		packet = ""
		for x in range(0,4) :
			packet += s.read()
			#print x
		
		if packet[0] != control_byte :
			print "Error: lost sync. Press any key to attempt to re-sync"
			os.system('pause')
			s.flushInput()
			get_lock()
		else :
			success = True
			

	return packet


print "Aquiring stream sync"
get_lock()

ACL_1_X_val = -1
ACL_1_Y_val = -1
ACL_1_Z_val = -1
GYRO_1_X_val = -1
GYRO_1_Y_val = -1
GYRO_1_Z_val = -1
buffer_size_max = 0
buffer_tick = 1
buffer_total = 1
sent_time = 0
received_time = 0
min_ping_time = 500
ping_tick = 1
ping_total = 1
wait_time = time.time() + 1
x_update = time.time()
x_period = 500
x_total = 1
x_tick = 1
UART_queue_len = 500
UART_queue_len_max = 0

while 1 :

	if (buffer_tick == 20) :
		buffer_tick /= 2
		buffer_total /= 2
		
	buffer_size = s.inWaiting()
	buffer_total += buffer_size
	buffer_tick += 1
	if buffer_size > buffer_size_max :
		buffer_size_max = buffer_size
		
	if (time.time() > wait_time) :
		s.write('P')
		sent_time = time.time()
		wait_time = sent_time + 1
		
	received_packet = get_packet()
	packet_count += 1
	device = ord(received_packet[1])

	os.system('cls')
	
	if device == ACL_1_X_addr :
		ACL_1_X_val = int(( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 ))
		if ACL_1_X_val > 32767 :
			ACL_1_X_val = (ACL_1_X_val-65536)
		
		if x_tick == 20 :
			x_tick /= 2
			x_total /= 2
		x_period = time.time() - x_update
		x_update = time.time()
		x_total += x_period
		x_tick += 1
	
	elif device == ACL_1_Y_addr :
		ACL_1_Y_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if ACL_1_Y_val > 32767 :
			ACL_1_Y_val = (ACL_1_Y_val-65536)

	elif device == ACL_1_Z_addr :
		ACL_1_Z_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if ACL_1_Z_val > 32767 :
			ACL_1_Z_val = (ACL_1_Z_val-65536)
			
	elif device == GYRO_1_X_addr :
		GYRO_1_X_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if GYRO_1_X_val > 32767 :
			GYRO_1_X_val = (GYRO_1_X_val-65536)
	
	elif device == GYRO_1_Y_addr :
		GYRO_1_Y_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if GYRO_1_Y_val > 32767 :
			GYRO_1_Y_val = (GYRO_1_Y_val-65536)
			
	elif device == GYRO_1_Z_addr :
		GYRO_1_Z_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if GYRO_1_Z_val > 32767 :
			GYRO_1_Z_val = (GYRO_1_Z_val-65536)
			
	elif device == ord('Q') :
		UART_queue_len = ord(received_packet[2])
		if (UART_queue_len > UART_queue_len_max) :
			UART_queue_len_max = UART_queue_len
		
			
	elif device == ord('P') :
		received_time = time.time()
		if ping_tick > 20 :
			ping_total /= 2
			ping_tick /= 2
			
		ping_total += received_time - sent_time
		ping_tick += 1
		
		if (received_time - sent_time) <  min_ping_time :
			min_ping_time = received_time - sent_time

	print "ACL X: %d" % (ACL_1_X_val)
	print "ACL Y: %d" % (ACL_1_Y_val)
	print "ACL Z: %d" % (ACL_1_Z_val)
	print "GYRO X: %d" % (GYRO_1_X_val)
	print "GYRO Y: %d" % (GYRO_1_Y_val)
	print "GYRO Z: %d" % (GYRO_1_Z_val)
	print "Minimum Ping Time: %lf" % (min_ping_time)
	print "Average Ping Time: %lf" % (ping_total/ping_tick)
	print "Sensor refresh period %lf" % (x_period)
	print "average refresh period %lf" % (x_total/x_tick)
	
	
	
	print "Maximum Buffer size: %d" % buffer_size_max
	print "Average Buffer size: %d" % (buffer_total/buffer_tick)
	print "UART queue length: %d" % UART_queue_len
	print "Max UART queue length: %d" % UART_queue_len_max
	print "Packet Count %d" % packet_count
	
	print "Run Time (minutes): %lf" % ((time.time() - start_time)/60)
	
	if abs(ACL_1_X_val) > 150  :
		s.write('1')
	elif abs(ACL_1_Y_val) > 150  :
		s.write('2')
	elif abs(ACL_1_Z_val) > 150  :
		s.write('4')
	


s.close()

