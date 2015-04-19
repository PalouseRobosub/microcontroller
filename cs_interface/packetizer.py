#!/usr/bin/python

import serial


class Packetizer():
    def __init__(self, Port, baudrate=115200, start_byte = 0x0A):
        self.start_byte = start_byte
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate

        if(type(Port) is int): #windows numerical port
            self.ser.port = Port - 1 
        elif(type(Port) is str): #linux string port
            self.ser.port = Port

        self.ser.open()

    def get_lock(self):
        current_byte = 0
        packet_array = []
        in_sync = False

        #flush the input buffer to
        #ensure we are reading the latest data
        self.ser.flushInput()
		
        print "Acquiring stream sync..."
        while in_sync == False:
			
            #get a single byte
            current_byte = ord(self.ser.read())

            #if byte was control byte, assume for now we are in sync
            if current_byte == self.start_byte:
                packet_array = [] #clear the array
                packet_array.append(current_byte)

                #read one more byte, should be the size byte
                size = ord(self.ser.read())

                #read <size> more bytes to get the rest of the packet
                self.ser.read(size)
				
                #read another byte, should be control_byte
                current_byte = ord(self.ser.read())

                #if we got a start byte, then we are fairly
                #sure we are in sync
                if (current_byte == self.start_byte):
                    size = ord(self.ser.read())
                    self.ser.read(size)
                    in_sync = True
                    print "sync locked"
					
        #end get_lock()

    def get_packet(self):
        success = False

        while success == False:
            #receive the first two bytes
            #should be start byte and size byte
            header = self.ser.read(2)
				
            #pull out the start and size bytes
            start = ord(header[0])
            size = ord(header[1])

            if (start != self.start_byte): #check that first byte is the start byte
                print "Error: lost sync"
                self.get_lock() #if not, get back into sync
            else: #we are in sync
                if(size > 0): #check that the packet's body isn't empty
                    chr_packet = self.ser.read(size) #read the rest of the packet
                    byte_packet = []
                for character in chr_packet: #convert chars to real bytes
                    byte_packet.append(ord(character))
                success = True

        return byte_packet #return the body of the packet (no header)
	#end get_packet()

	def send_packet(self, data):
		packet = []

		packet.append(chr(self.start_byte)) #add the start byte
		packet.append(chr(len(data)))	   #add the size
		#convert all the data byte into chars
		for byte in data:
			packet.append(chr(byte))

		str_packet = ''.join(packet)

		#send the packet
		self.ser.write(str_packet)
	#end send_packet()

        	
