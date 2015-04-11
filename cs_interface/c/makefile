
OPTIONS=-Wall
CC=gcc


all: write_serial.exe read_serial.exe


serial.o: serial.c serial.h
	$(CC) $(OPTIONS) -c serial.c

write_serial.exe: serial.o write_serial.c
	$(CC) $(OPTIONS) serial.o write_serial.c -o write_serial.exe

read_serial.exe: serial.o read_serial.c
	$(CC) $(OPTIONS) serial.o read_serial.c -o read_serial.exe

clean:
	rm -rf *.o *.exe
