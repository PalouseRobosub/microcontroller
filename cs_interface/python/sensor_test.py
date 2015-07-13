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
start_switch = 6

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
	a_count = 0
	g_count = 0
	m_count = 0
	d_count = 0
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
	start_switch_data = 0
		

	p.get_lock()
	while(1):
		data = p.get_packet()
		if (data[0] == accel): # Accelerometer Data
			a_count += 1
			accel_x = convert(data[2], data[1])
			accel_y = convert(data[4], data[3])
			accel_z = convert(data[6], data[5])
		elif (data[0] == gyro): # Gyroscope Data
			g_count += 1
			gyro_x = convert(data[2], data[1])
			gyro_y = convert(data[4], data[3])
			gyro_z = convert(data[6], data[5])
		elif (data[0] == magnometer): # Magnometer Data
			m_count += 1
			magno_x = convert(data[2], data[1])
			magno_y = convert(data[4], data[3]) #Y value comes after Z value
			magno_z = convert(data[6], data[5])
		elif (data[0] == sid_depth): # depth sensor Data
			d_count += 1
			depth_data = (data[2] << 8) | data[1];
		elif (data[0] == start_switch):
			
			start_switch_data = data[1]

		os.system('clear')

		# Now print out the values
		print 'Accelerometer: \tX={}\tY={}\tZ={}\tcount={}' .format(accel_x, accel_y, accel_z, a_count)
		print 'Gryoscope: \tX={}\tY={}\tZ={}\tcount={}' .format(gyro_x, gyro_y, gyro_z, g_count)
		print 'Magnetometer: \tX={}\tY={}\tZ={}\tcount={}' .format(magno_x, magno_y, magno_z, m_count)
		print 'Depth: {}, count={}'.format(depth_data, d_count)
		print 'Start: {}'.format(start_switch_data)

# Junk required to call main first
if __name__ == "__main__":
		main()

