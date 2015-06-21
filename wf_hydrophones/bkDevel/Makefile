CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
LIBS=-ldwf -lstdc++
SOURCES=read_hydrophones.cpp ADInterface.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test.out

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
