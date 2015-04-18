#!/usr/bin/python
from packetizer import Packetizer

p = Packetizer('/dev/ttyUSB3')

while(1):
    bytes=p.get_packet()
    print bytes

