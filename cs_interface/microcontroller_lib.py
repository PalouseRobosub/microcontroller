#!/usr/bin/python
"""
This is the microcontroller library. All classes and functions that interact
with the microcontrollers that might be shared across different micrcocontroller
modules should (eventually) go in here.
"""
import os

class Thruster:
    """
    This class is used for controlling the thrusters. An instance should be
    made for each thruster.

    Initialization Example:
        mthruster = Thruster(37, 1)
        
    Several parameters must be passed when initializing the thruster.
    
    Address:
        The address of the H-bridge that the thruster is connected to.
    H-bridge Port:
        The port of the  H-bridge that the thruster is connected to.
        Valid values are 1 and 2.
    Serial Port:
        A handle to a serial port with which it can communicate to the thrusters
    

    Usage:s
        Once initialized, thruster commands can be sent using the .send() method.
    
    
    
    ***How the Thrusters/H-bridges work***
            (if you're curious)
    
    The thrusters are controlled by the H-bridges, which has a specific
    control protocol. Each H-bridge controls 2 thrusters (named 1 and 2,
    respectively). To control a thruster, one must send the address of the 
    H-bridge, a function code(explained later), the magnitude of the thruster 
    value, and a special checksum computed of the previous parameters.
    
    H-bridge communication parameters:
    
    Address:
        The address corresponds to the H-bridge controlling the thruster,
        not the thruster itself. For six thrusters, there are 3 H-bridges. 
        Therefore, each thruster will share the same address as one other
        thruster.
    
    Function Code:
        There are 4 function codes that we use. Look at the datasheet for
        more information about other function codes.
        
        Code 0:
            Drive motor 1 forwards
        Code 1:
            Drive motor 1 backwards
        Code 4:
            Drive motor 2 forwards
        Code 5:
            Drive motor 2 backwards
    
    Magnitude:
        The value to be sent to the H-bridge. Valid values are between 0 and 127.
        These will cause a thruster to go forwards or backwards depending on the
        function code that is sent. With either function code, a magnitude of 0 will
        cause the thruster to stop.
    
    Checksum:
        The checksum is used by the H-bridge to validate sent data. The checksum is
        calculated as follows:
            checksum = (Addresss + Function Code + Magnitude) & 127 
    """
    
    #external class members
    address = 0
    h_bridge_port = 0
    serial_port = 0
    
    #internal class members (you shouldn't touch these)
    #raw_forward_code
    #raw_backward_code
    
    def __init__(self, address, h_bridge_port, serial_port) :
        self.address = address
        self.h_bridge_port = h_bridge_port
        self.serial_port = serial_port
    
    def __setattr__(self, name, value) :
        if (name == "address") :
            self.__dict__[name] = value
            self.__dict__["raw_address"] = chr(value)
            
        elif (name == "h_bridge_port") :
            if (value == 1) :
                self.__dict__[name] = value
                self.__dict__["raw_forward_code"] = chr(0)
                self.__dict__["raw_backward_code"] = chr(1)
            elif (value == 2) :
                self.__dict__[name] = value
                self.__dict__["raw_forward_code"] = chr(4)
                self.__dict__["raw_backward_code"] = chr(5)
            else :
                print "Error: Invalid H-bridge port selected"
                
        if (name == "serial_port") :
            self.__dict__[name] = value
    
    def __repr__ (self):
    
        mstr = "Address: " + str(self.address) + '\n' \
               "H-bridge Port:    " + str(self.h_bridge_port) + '\n'
               
        return mstr         
    
    #class methods
    def send(self, value):
        
        #ensure input is within bounds
        if (value > 127) :
            value = 127
        elif (value < -127) :
            value = -127
            
        #set the direction
        if (value > 0) :
            raw_function_code = self.raw_forward_code
        else :
            raw_function_code = self.raw_backward_code
        
        # calculate absolute value
        value = int(abs(value))
        
        #calculate the checksum
        checksum = ( (ord(self.raw_address) + ord(raw_function_code) + value) & 127)
        
        #generate the entire command
        cmd = self.raw_address + raw_function_code + chr(value) + chr(checksum)
        
        # os.system('pause')
        # print ("address " + str(ord(cmd[0])))
        # self.serial_port.write(cmd[0])
        # os.system('pause')
        # print ("function code " + str(ord(cmd[1])))
        # self.serial_port.write(cmd[1])
        # os.system('pause')
        # print ("value " + str(ord(cmd[2])))
        # self.serial_port.write(cmd[2])
        # os.system('pause')
        # print ("checksum " + hex(ord(cmd[3])))
        # self.serial_port.write(cmd[3])
        # print (" ")
        self.serial_port.write(cmd)
        
    #end send()
#end thruster class


class Sensors:
    """
    This class is designed for interpreting serial packets from the microcontroller and
    sending the data out on the grapevine. The class does little work itself, it needs to
    be passed all functions that are called. It is in essence, the glue holding things
    together. It's more of a c-style struct, really.
    """

    #sensor descriptors
    ACL_0_X  = 0x10
    ACL_0_Y  = 0x11
    ACL_0_Z  = 0x12

    ACL_1_X  = 0x13
    ACL_1_Y  = 0x14
    ACL_1_Z  = 0x15

    GYRO_0_X = 0x20
    GYRO_0_Y = 0x21
    GYRO_0_Z = 0x22

    MAG_0_X  = 0x40
    MAG_0_Y  = 0x41
    MAG_0_Z  = 0x42

    MAG_1_X  = 0x43
    MAG_1_Y  = 0x44
    MAG_1_Z  = 0x45

    ADC_DEPTH = 0x30
    ADC_BATT  = 0x31

    START_SW = 0x50

 
    #class members
    
    #dictionary of functions for processing functions
    processor_functions = {}

    #dictionary of functions for processing functions
    sensor_vals = {}

    #dictionary of functions for processing functions
    data_formatters = {}

    #callback functions
    callbacks = {}
    
    #contructor function
    def __init__(self) :
        
        #initalize the processor functions
        #ACL 0
        self.processor_functions[self.ACL_0_X] = self.generic_sensor_handler
        self.processor_functions[self.ACL_0_Y] = self.generic_sensor_handler
        self.processor_functions[self.ACL_0_Z] = self.generic_sensor_handler
        #ACL 1
        self.processor_functions[self.ACL_1_X] = self.generic_sensor_handler
        self.processor_functions[self.ACL_1_Y] = self.generic_sensor_handler
        self.processor_functions[self.ACL_1_Z] = self.generic_sensor_handler
        #GYRO 0
        self.processor_functions[self.GYRO_0_X] = self.generic_sensor_handler
        self.processor_functions[self.GYRO_0_Y] = self.generic_sensor_handler
        self.processor_functions[self.GYRO_0_Z] = self.generic_sensor_handler
        #MAG 0
        self.processor_functions[self.MAG_0_X] = self.generic_sensor_handler
        self.processor_functions[self.MAG_0_Y] = self.generic_sensor_handler
        self.processor_functions[self.MAG_0_Z] = self.generic_sensor_handler
        #MAG 1
        self.processor_functions[self.MAG_1_X] = self.generic_sensor_handler
        self.processor_functions[self.MAG_1_Y] = self.generic_sensor_handler
        self.processor_functions[self.MAG_1_Z] = self.generic_sensor_handler
        #ADC devices
        self.processor_functions[self.ADC_DEPTH] = self.generic_sensor_handler
        self.processor_functions[self.ADC_BATT] = self.generic_sensor_handler
        #Mission Start switch
        self.processor_functions[self.START_SW] = self.start_switch_handler
        
        #initalize the data formatters
        #ACL 0
        self.data_formatters[self.ACL_0_X] = self.zls
        self.data_formatters[self.ACL_0_Y] = self.zls
        self.data_formatters[self.ACL_0_Z] = self.zls
        #ACL 1
        self.data_formatters[self.ACL_1_X] = self.zls
        self.data_formatters[self.ACL_1_Y] = self.zls
        self.data_formatters[self.ACL_1_Z] = self.zls
        #GYRO 0
        self.data_formatters[self.GYRO_0_X] = self.zms
        self.data_formatters[self.GYRO_0_Y] = self.zms
        self.data_formatters[self.GYRO_0_Z] = self.zms
        #MAG 0
        self.data_formatters[self.MAG_0_X] = self.zms
        self.data_formatters[self.MAG_0_Y] = self.zms
        self.data_formatters[self.MAG_0_Z] = self.zms
        #MAG 1
        self.data_formatters[self.MAG_1_X] = self.zms
        self.data_formatters[self.MAG_1_Y] = self.zms
        self.data_formatters[self.MAG_1_Z] = self.zms
        #ADC devices
        self.data_formatters[self.ADC_DEPTH] = self.zls_u
        #Mission Start switch
        self.data_formatters[self.ADC_BATT] = self.zls_u
        
        #initialize all sensor values
        #ACL 0
        self.sensor_vals[self.ACL_0_X] = 0
        self.sensor_vals[self.ACL_0_Y] = 0
        self.sensor_vals[self.ACL_0_Z] = 0
        #ACL 1
        self.sensor_vals[self.ACL_1_X] = 0
        self.sensor_vals[self.ACL_1_Y] = 0
        self.sensor_vals[self.ACL_1_Z] = 0
        #GYRO 0
        self.sensor_vals[self.GYRO_0_X] = 0
        self.sensor_vals[self.GYRO_0_Y] = 0
        self.sensor_vals[self.GYRO_0_Z] = 0
        #MAG 0
        self.sensor_vals[self.MAG_0_X] = 0
        self.sensor_vals[self.MAG_0_Y] = 0
        self.sensor_vals[self.MAG_0_Z] = 0
        #MAG 1
        self.sensor_vals[self.MAG_1_X] = 0
        self.sensor_vals[self.MAG_1_Y] = 0
        self.sensor_vals[self.MAG_1_Z] = 0
        #ADC devices
        self.sensor_vals[self.ADC_DEPTH] = 0
        self.sensor_vals[self.ADC_BATT] = 0
        #Mission Start switch
        self.sensor_vals[self.START_SW] = 0

    #end __init__

    def process_packet(self, packet) :
		
        #pull apart the packet
        descriptor = ord(packet[1])
        byte_0 = ord(packet[2])
        byte_1 = ord(packet[3])

        #process the packet
        self.processor_functions[descriptor](descriptor, byte_0, byte_1)
        
        #call the callback function (if it exists)
        if (self.callbacks.has_key(descriptor)):
            self.callbacks[descriptor]()
            #print "calling callback", hex(descriptor)
        
    #end process_packet
    
    #helper methods
    
    def generic_sensor_handler(self, descriptor, byte_0, byte_1) :
        #extract the data from the packet
        self.sensor_vals[descriptor] = self.data_formatters[descriptor](byte_0, byte_1)
        
        #send over grapevine
        #self.grapevine_handles[descriptor].publish_message({self.grapevine_names[descriptor]: self.sensor_vals[descriptor]) 
    #end generic_sensor


    def start_switch_handler(self, descriptor, byte_0, byte_1) :
        #extract the data from the packet, increment counter
        self.sensor_vals[descriptor] = self.sensor_vals[descriptor] + 1 
        
        #send over grapevine
        #self.grapevine_handles[descriptor].publish_message({self.grapevine_names[descriptor]: self.sensor_vals[descriptor]) 
    #end generic_sensor

    #zms stands for "zero byte most significant"
    def zms(self, byte_0, byte_1) :
        
        value = ( (byte_0 << 8) | byte_1)
        
        if (value > 32767) :
            value = value - 65536
        
        return value
    #end zms

    #zls stands for "zero byte least significant"
    def zls(self, byte_0, byte_1) :
        
        value = ( (byte_1 << 8) | byte_0 )      
        
        if (value > 32767) :
            value = value - 65536

        return value
    #end zls

    #zls function, but unsigned
    def zls_u(self, byte_0, byte_1) :
        return ( (byte_1 << 8) | byte_0 )           
    #end zls_u


    

#end sensor class

"""
The following functions are used for processing packets. These functions should never
be called directly, they should always get linked to an instance of the Sensor class
""" 


#class for the actuator board
class actuator:
    #external class members
    torpedo_L_id = 0x21
    torpedo_R_id = 0x20
    stepper_front_id = 0x26
    stepper_bottom_id = 0x27
    
    #special class methods
    def __init__(self, serial_port):
        self.serial_port = serial_port
    
    def __setattr__(self, name, value):                    
        if (name == "serial_port"):
            self.__dict__[name] = value
    

    #class methods
    def fire_torpedo(self, which):
        if (which == "left"):
            raw_device_id = chr(self.torpedo_L_id)
        elif (which == "right"):
            raw_device_id = chr(self.torpedo_R_id)
        else: #error condition
            print "error: torpedo descriptor not recognized"
            raw_device_id = chr(0x00)
        
        #generate the entire command
        cmd = '\n' + raw_device_id + chr(0x00)
        
        self.serial_port.write(cmd)
    #end fire_torpedo

    def drop_marker(self, which):

        raw_device_id = chr(self.stepper_bottom_id)
        
        if (which == "left"):
            raw_command = chr(0xF1)
        elif (which == "right"):
            raw_command = chr(0xF2)
        else: #error condition
            print "error: marker descriptor not recognized"
            raw_device_id = chr(0x00)
        
        #generate the entire command
        cmd = '\n' + raw_device_id + raw_command
        
        self.serial_port.write(cmd)
    #end drop_marker



    def reset_stepper(self, which):

        reset_command = 0xF0

        if (which == "bottom"):
            raw_device_id = chr(self.stepper_bottom_id)
        elif (which == "front"):
            raw_device_id = chr(self.stepper_front_id)
        else: #error condition
            print "error: stepper motor descriptor not recognized"
            raw_device_id = chr(0x00)


        #generate the entire command
        cmd = '\n' + raw_device_id + chr(reset_command)
        
        self.serial_port.write(cmd)
    #end reset_stepper


    def enable_steppers(self, enabled):

        enable_command = 0xF5
        disable_command = 0xF6

        if (enabled == True):
            command = enable_command
        elif (enabled == False):
            command = disable_command
        else:
            print 'error: improper enable value. Accepted values are \"True\"(1) and \"False\"(0)'

        #it doesn't matter which, enabling/disabling either will
        #effect both
        raw_device_id = chr(self.stepper_bottom_id)


        #generate the entire command
        cmd = '\n' + raw_device_id + chr(command)
        
        self.serial_port.write(cmd)
    #end enable_steppers


    def close_claw(self, which):

        close_command = 0xF3

        if (which == "bottom"):
            raw_device_id = chr(self.stepper_bottom_id)
        elif (which == "front"):
            raw_device_id = chr(self.stepper_front_id)
        else: #error condition
            print "error: claw descriptor not recognized"
            raw_device_id = chr(0x00)


        #generate the entire command
        cmd = '\n' + raw_device_id + chr(close_command)
        
        self.serial_port.write(cmd)
    #end close_claw

    def stop_claw(self, which):

        stop_command = 0xF4

        if (which == "bottom"):
            raw_device_id = chr(self.stepper_bottom_id)
        elif (which == "front"):
            raw_device_id = chr(self.stepper_front_id)
        else: #error condition
            print "error: claw descriptor not recognized"
            raw_device_id = chr(0x00)


        #generate the entire command
        cmd = '\n' + raw_device_id + chr(stop_command)
        
        self.serial_port.write(cmd)
    #end stop claw
    
    #alias for open_claw function
    open_claw = reset_stepper 





                    
#end actuator class
