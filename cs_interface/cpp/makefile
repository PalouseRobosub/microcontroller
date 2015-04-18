OPTIONS=-Wall
CC=g++
OBJECTS=serial.o packetizer.o


#test: write_serial.exe read_serial.exe

lib: $(OBJECTS)

#build all source files
%.o : %.cpp %.c
	$(CC) $(OPTIONS) -c $<
clean:
	rm -rf *.o *.exe

#special builds
write_serial.exe: $(OBJECTS) write_serial.cpp
	$(CC) $(OPTIONS) serial.o write_serial.cpp -o write_serial.exe

read_serial.exe: $(OBJECTS) read_serial.cpp
	$(CC) $(OPTIONS) serial.o read_serial.cpp -o read_serial.exe


serial.o: serial.h
packetizer.o: packetizer.h serial.h



