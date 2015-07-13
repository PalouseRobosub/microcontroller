OPTIONS=-Wall -Wno-char-subscripts -g
CC=g++
#OBJECTS=serial.o packetizer.o send_packet.o receive_packet.o

all: test

lib: serial.o packetizer.o

test: write_serial.exe read_serial.exe receive_packet.exe send_packet.exe

clean:
	rm -rf *.o *.exe

#special builds
write_serial.exe: serial.o write_serial.o
	$(CC) $(OPTIONS) serial.o write_serial.cpp -o write_serial.exe

read_serial.exe: serial.o read_serial.o
	$(CC) $(OPTIONS) serial.o read_serial.cpp -o read_serial.exe

receive_packet.exe: packetizer.o receive_packet.o	
	$(CC) $(OPTIONS) serial.o packetizer.o receive_packet.o -o receive_packet.exe

send_packet.exe: packetizer.o send_packet.o	
	$(CC) $(OPTIONS) serial.o packetizer.o send_packet.o -o send_packet.exe

serial.o: serial.h serial.cpp
	$(CC) $(OPTIONS) -c serial.cpp 
packetizer.o: serial.o packetizer.h packetizer.cpp 
	$(CC) $(OPTIONS) -c packetizer.cpp
send_packet.o: packetizer.o send_packet.cpp 
	$(CC) $(OPTIONS) -c send_packet.cpp
receive_packet.o: packetizer.o receive_packet.cpp 
	$(CC) $(OPTIONS) -c receive_packet.cpp

