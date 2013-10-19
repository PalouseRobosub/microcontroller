#imports
import serial

#constants
control_byte = '\n'





"""
Here we are trying to make sure we have actually found
a control byte, so we receive several packets, then look
at where we expect the control bytes to be. If they are not in the expected
locastions, we wait for a new control byte and try again.

X000X000X
012345678
"""
#varialbles for the sync loop
mchar = '\0'
mstring = ""
in_sync = False
while in_sync == False:
    #mchar = ser.read()
    mchar = control_byte
    if mchar == control_byte :
        mstring = ""
        mstring += mchar

        #receive several packets
        while len(mstring) != 8 :
            #mstring += ser.read()

        if (mstring[0] == control_byte and \
            mstring[4] == control_byte and \
            mstring[7] == control_byte) : #we have sync

            #throw away rest of last packet
            #ser.read()
            #ser.read()
            #ser.read()

            #say we are in sync so we can break out
            in_sync = True

        #don't need an else case, we just start loop again
            


        
        
