#!/usr/bin/python
from packetizer import Packetizer
import os
import signal
import serial
accel = 0
gyro = 1
magnometer = 2
temperature = 3
pressure = 4
sid_depth = 5

# Variables for printing
p = Packetizer('/dev/ttyUSB2')

# This function will convert two bits into a single value
def convert(char1, char2):
		bits = 16
		char1 = char1 << 8
		char1 = char1 | char2
		if (char1 & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
			char1 = char1 - (1 << bits)		   # compute negative value
		return char1

# The main program
def main():
	accel_x = 0
	accel_y = 0
	accel_z = 0
	gyro_x = 0
	gyro_y = 0
	gyro_z = 0
	magno_x = 0
	magno_y = 0
	magno_z = 0
	temp = 0
	press = 0
	depth_data = 0;

	p.get_lock()
	while(1):
		data = p.get_packet()
		if (data[0] == accel): # Accelerometer Data
			accel_x = convert(data[2], data[1])
			accel_y = convert(data[4], data[3])
			accel_z = convert(data[6], data[5])
		elif (data[0] == gyro): # Gyroscope Data
			gyro_x = convert(data[2], data[1])
			gyro_y = convert(data[4], data[3])
			gyro_z = convert(data[6], data[5])
		elif (data[0] == magnometer): # Magnometer Data
			magno_x = convert(data[2], data[1])
			magno_y = convert(data[6], data[5]) #Y value comes after Z value
			magno_z = convert(data[4], data[3])
		elif (data[0] == sid_depth): # depth sensor Data
			depth_data = (data[1] << 8) | data[2];

		os.system('clear')

		# Now print out the values
		print 'Accelerometer: \tX={}\tY={}\tZ={}' .format(accel_x, accel_y, accel_z)
		print 'Gryoscope: \tX={}\tY={}\tZ={}' .format(gyro_x, gyro_y, gyro_z)
		print 'Magnetometer: \tX={}\tY={}\tZ={}' .format(magno_x, magno_y, magno_z)
		print 'Depth: {}' .format(depth_data)

# Junk required to call main first
if __name__ == "__main__":
		main()

