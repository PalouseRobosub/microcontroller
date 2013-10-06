import serial

ser = serial.Serial(); #open up an instance of the serial module
ser.baudrate = 56818 #set baud rate
ser.port = 2 #changes depending on what port we're using
ser.open() #opens the serial port
#ser.read() #reads one byte
#ser.write('a') #writes one byte



#this code just reads a newline-terminated line and prints it to the display
mstring = ""
mchar  = '0'
while 1:
    mchar = ser.read();
    if mchar != '\n' :
        mstring += mchar
    else:
        print mstring
        mstring = ""
        
