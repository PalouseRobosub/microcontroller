#!/usr/bin/python
from packetizer import Packetizer
import time

def main():
	p = Packetizer('/dev/ttyUSB1')
	pneumatic = 0
	packet = [0 , 1]
	while (1):
		if (packet[1] == 1):
			packet[1] = 0
		else:
			packet[1] = 1
		pneumatic = 0
		while 1:
			packet[0] = pneumatic
			p.send_packet(packet)
			pneumatic += 1
			time.sleep(.1)
			if pneumatic == 8:
				break

if __name__ == "__main__":
	main()
