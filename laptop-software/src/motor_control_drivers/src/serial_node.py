#!/usr/bin/python
import random
import rospy
import serial
import sys
import struct
import string

from motor_control_drivers.msg import  BuddySerial

class serial_node:
    def __init__(self, node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        """
        publish rate in herts
        subscription name is the topic to which it subscribe to, from the data it will return to hercules, can be list
        no node name will create a fake node to publish random data
        
        This node once started, will read from serial. Will publish data as it arrives and it's valid flag is set
        """
        self.serial_buffer='' #buffer that holds incoming data
        self.serial_packet='' #packet of 18 bytes to be processed
        self.flg_rdy_to_pub=False #flag if data is valid to pubish
        self.DEBUG_EN=DEBUG_EN
        self.rosRate=None #obj for the rospy.Rate()

        if publish_rate is None: publish_rate = 100
        if queue_size is None: queue_size = 10
        

        if subscription_names is not None and isinstance(subscription_names, list): # if it is passed and a list
            print("Subscribed to: ")
            for i in subscription_names:
                print(i)
        elif subscription_names is not None and isinstance(subscription_names, basestring): # if it is passed and just a string
            print("Subscribed to: %s"%subscription_names)
        else:
            print("Not subscribing to anything")
            
        #create publisher  and the serial initialization  
        if node_name is None or serial_port_str is None: #creation of the node
            print("Creating fake node and publish fake data")
            rospy.init_node("Fake_serial_node")
            rospy.is_shutdown()
            self.pub=rospy.Pulisher("Fake_serial_data_topic", BuddySerial, queue_size=queue_size)
        else:
            self.ser=initSerial()  #initialize serial port
            rospy.init_node(node_name)
            rospy.is_shutdown()
            self.pub=rospy.Pulisher("BuddySerialParsed_topic", BuddySerial, queue_size=queue_size)

        
        self.rosRate=rospy.Rate(publish_rate)        
        if self.ser is None:
            print("ERROR with serial port, cannot open, will generate fake data")
    
    def startNode(self):
        """
        starts the node running to receive and send out data by publishing and subscribing
        """
        while True:
            if self.ser is None: #generate fake data
                self.serial_buffer = "ff"+"".join(random.sample(string.ascii_lowercase, 16)) # create 18 'bytes' of data of random char
            else:
                self.readSerial2Buffer(self.ser)
            if self.DEBUG_EN:
                print("SerialBuffer: "+self.serial_buffer)
                print_data()
            self.clearSerialBuffer()
            
            self.pkt_process()
            
            if self.flg_rdy_to_pub:
                self.pub.publish(self.parsed_serial_data)
            else:
                print("Not ready to publish")
            self.rosRate.sleep()

    
    def initSerial(self, serial_dev_str=None, baud_rate=None):
        """
        Initialize the physical serial port to the Hercules
        """
        if baud_rate is None: baud_rate = 9600
        if serial_dev_str is None: serial_dev_str = 'ttyUSB0'
        try:
          ser = serial.Serial()
          ser.baurdrate = baud_rate
          ser.port = '/dev/%s'%serial_dev_str
          ser.timeout=0 #non blocking mode
          ser.open()
          return ser
        except serial.serialutil.SerialException:
            print("Cannot Open serial port %s with baud rate %s" %(serial_dev_str, baud_rate)
      
        return None

    def readSerial2Buffer(self, serial_inst):
        """
        append new data into serial buffer
        pass in the serial instance to read from
        """
        if isinstance(serial_inst, serial.Serial): #check if it is a serial port, and read from it
            self.serial_buffer+=serial_inst.read(198) #read 19 bytes
        else:
            print("No serial instance passed")
            return 1
    
    def clearSerialBuffer(self):
        self.serial_buffer=""
    
 
    def pkt_process(self):
        """
        check if buffer has more than 18 bytes, then sort bytes into strings
        """
        self.flg_rdy_to_pub = False
        
        while self.serial_buffer[0:1] != 'ff' and len(self.serial_buffer)>2:
            print("ERROR: serial buffer header not ff, shifting it: %s"%self.serial_buffer.pop[0:17])
            #shift the buffer by 1
            self.serial_buffer.pop(1) 

        if len(self.serial_buffer)<18:
            return 1

        self.serial_packet = self.serial_buffer.pop(18)
        
        self.Packetheader = ord(serialbuffer[0])
        self.ValidData = ord(serialbuffer[1])
        self.UltraF = ord(serialbuffer[2])
        self.UltraB = ord(serialbuffer[3])
        self.EncL = stringtofloat(serialbuffer[4:8])
        self.EncR = stringtofloat(serialbuffer[8:12])
        self.Infra = [ord(serialbuffer[12]), 
           ord(serialbuffer[13]), 
           ord(serialbuffer[14]), 
           ord(serialbuffer[15]), 
           ord(serialbuffer[16]), 
           ord(serialbuffer[17])]
           
        #fill up the info to be published
        self.parsed_serial_data= BuddySerial(Packetheader=self.Packetheader, ValidData=self.ValidData
                                        UltraF=self.UltraF, UltraB=self.UltraF,
                                        EncL=self.EncL, EncR=self.EncR,
                                        Infra0=self.Infra0, Infra1=self.Infra1, Infra2=self.Infra2,
                                        Infra3=self.Infra3, Infra4=self.Infra4, Infra5=self.Infra5)
        rospy.loginfo(self.parsed_serial_data)
        self.flg_rdy_to_pub = True
        return 0
 
    def print_data(self):
        """
        used for debug, print out the data
        """
        print("DEBUG Packet Print - Buffer size: %s" %len(self.serial_buffer))
        print("Packet Header: %s, Valid Data: %s" %(self.Packetheader, self.ValidData))
        print("EncoderL: %s, EncoderR: %s"%(self.EncL, self.EncR))
        print("UltraF: %s, UltraB: %s"%(self.UltraF, self.UltraB))
        print("Infra: "),
        print(self.infra)
        
def stringtofloat4(string):
    """
    4 character input
    little endian input
    """
    if len(string) != 4:
        print("ERROR: string %s is not 4 char long"%string)
        raise
        return -1
    else:
        return struct.unpack('>f', string[::-1])[0]
        
    return 0
  
if __name__=='__main__':
    print("Trying to read from serial and publish (fake)? data")
    
    try:
    #self, node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        FakeSerial=serial_node(node_name='FakeSerial', serial_port_str='ttyUSB0', DEBUG_EN=True)
        FakeSerial.startNode()
    except rospy.ROSInterruptException:
        pass
