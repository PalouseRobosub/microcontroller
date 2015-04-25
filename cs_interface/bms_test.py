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

