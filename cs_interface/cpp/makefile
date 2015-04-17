
OPTIONS=-Wall
CC=g++


test: write_serial.exe read_serial.exe

lib: write_serial.exe

serial.o: serial.cpp serial.h
	$(CC) $(OPTIONS) -c serial.cpp

write_serial.exe: serial.o write_serial.cpp
	$(CC) $(OPTIONS) serial.o write_serial.cpp -o write_serial.exe

read_serial.exe: serial.o read_serial.cpp
	$(CC) $(OPTIONS) serial.o read_serial.cpp -o read_serial.exe

clean:
	rm -rf *.o *.exe
