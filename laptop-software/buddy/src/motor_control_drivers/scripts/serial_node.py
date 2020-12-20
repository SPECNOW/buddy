#!/usr/bin/python
import random
import rospy
import serial
import sys
import struct
import string
import time

from motor_control_drivers.msg import BuddySerial

class serial_node:
    def __init__(self, node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        """
        publish rate in herts
        subscription name is the topic to which it subscribe to, from the data it will return to hercules, can be list
        no node name will create a fake node to publish random data
        
        This node once started, will read from serial. Will publish data as it arrives and it's valid flag is set
        """
        serial_port_str = rospy.get_param("comPort", "") 
        publish_rate = int(rospy.get_param("publishRate", "100.0"))
        publish_topic = rospy.get_param("topicOut", "Fake_serial_data_topic")
        self.DEBUG_EN = rospy.get_param("debugEnable", "True").lower() == "true"
        
        self.serial_buffer = ''     #buffer that holds incoming data
        self.serial_packet = ''     #packet of 18 bytes to be processed
        self.flg_rdy_to_pub = False #flag if data is valid to pubish
        self.rosRate = None         #obj for the rospy.Rate()
        self.ser = None
        
        self.last_time = None # rospy.Time.now()        
        self.Packetheader = \
        self.ValidData = \
        self.UltraF = \
        self.UltraB = \
        self.EncL = \
        self.EncR = ""
        self.Infra = [""]*6

        if publish_rate is None: 
            publish_rate = 100
        if queue_size is None: 
            queue_size = 0

        if subscription_names is not None and isinstance(subscription_names, list): # if it is passed and a list
            rospy.logdebug("Subscribed to: ")
            for i in subscription_names:
                rospy.logdebug(i)
        elif subscription_names is not None and isinstance(subscription_names, basestring): # if it is passed and just a string
            rospy.logdebug("Subscribed to: %s"%subscription_names)
        else:
            rospy.logdebug("Not subscribing to anything")
            
        #create publisher  and the serial initialization  
        if node_name is None or serial_port_str is None: #creation of the node
            rospy.loginfo("Creating fake node and publish fake data")
            rospy.init_node("Fake_serial_node", log_level=rospy.DEBUG if self.DEBUG_EN else rospy.INFO)
            rospy.is_shutdown()
            self.pub=rospy.Publisher(publish_topic, BuddySerial, queue_size=queue_size)
        else:
            self.ser=self.initSerial(serial_port_str,)  #initialize serial port
            rospy.init_node(node_name, log_level=rospy.DEBUG if self.DEBUG_EN else rospy.INFO)
            rospy.is_shutdown()
            self.pub=rospy.Publisher(publish_topic, BuddySerial, queue_size=queue_size)

        rospy.logerr("RosRate for Serial is: %s", str(publish_rate))
        self.rosRate=rospy.Rate(publish_rate)        
        if self.ser is None:
            rospy.logerr("ERROR with serial port, cannot open, will generate fake data")
    
    def startNode(self):
        """
        starts the node running to receive and send out data by publishing and subscribing
        """
        while not rospy.is_shutdown():
            if self.ser is None: #generate fake data
                self.serial_buffer = "00\xff"+"".join(random.sample(string.ascii_lowercase, 17)) # create 18 'bytes' of data of random char
            else:
                self.readSerial2Buffer(self.ser)
           
            if self.DEBUG_EN:
                rospy.logdebug("SerialBuffer: "+ self.serial_buffer)
                self.print_data()
            # self.clearSerialBuffer()
            
            self.pkt_process()
            
            if self.flg_rdy_to_pub:
                self.pub.publish(self.parsed_serial_data)
            else:
                rospy.logdebug("Not ready to publish")
            self.rosRate.sleep()

    
    def initSerial(self, serial_dev_str=None, baud_rate=None):
        """
        Initialize the physical serial port to the Hercules
        """
        if baud_rate is None: 
            baud_rate = 1152000
        if serial_dev_str is None: 
            serial_dev_str = 'ttyUSB0'
        try:
          ser = serial.Serial()
          ser.baurdrate = baud_rate
          ser.port = '/dev/%s'%serial_dev_str
          ser.timeout=0 #non blocking mode
          ser.open()
          return ser
        except Exception:
            #rospy.logerr("Cannot Open serial port %s with baud rate %s" %(serial_dev_str, baud_rate))
            pass
        return None

    def readSerial2Buffer(self, serial_inst):
        """
        append new data into serial buffer
        pass in the serial instance to read from
        """
        if isinstance(serial_inst, serial.Serial): #check if it is a serial port, and read from it
            self.last_time = rospy.Time.now()
            self.ser.write('ss')
            self.serial_buffer+=serial_inst.read(40-len(self.serial_buffer)) #read 20 bytes
        else:
            rospy.logerr("No serial instance passed")
            return 1
    
    def clearSerialBuffer(self):
        self.serial_buffer=""
    
 
    def pkt_process(self):
        """
        check if buffer has more than 18 bytes, then sort bytes into strings
        """
        self.flg_rdy_to_pub = False
        
        while self.serial_buffer[0:1].lower() != '\xff' and len(self.serial_buffer) > 18:
            rospy.logerr("ERROR: serial buffer header not 0xFF, shifting it: %s, lenght is %s" % (self.serial_buffer[0:17], str(len(self.serial_buffer))))
            buf=''
            for char in self.serial_buffer:
                buf=buf + char.__repr__()
            rospy.logerr("ERROR: Buffer is %s" % buf)
            #shift the buffer by 1
            if self.serial_buffer[0:1].lower() != '\xff':
                self.serial_buffer = self.serial_buffer[1:]

        if len(self.serial_buffer) < 18:
            rospy.logerr("ERROR: serial package is too short. Length is only " + str(len(self.serial_buffer)))
            return 1

        self.serial_packet = self.serial_buffer[0:18]
        self.serial_buffer = self.serial_buffer[20:]
        
        self.Packetheader = ord(self.serial_packet[0])
        self.ValidData = ord(self.serial_packet[1])
        self.UltraF = ord(self.serial_packet[2])
        self.UltraB = ord(self.serial_packet[3])
        # TODO: FIX DIS UGLIENESS, converting from 4 
        self.EncL = (ord(self.serial_packet[4])<<0) + (ord(self.serial_packet[5])<<8) + (ord(self.serial_packet[6])<<16) + (ord(self.serial_packet[7]) << 24)
        self.EncR = (ord(self.serial_packet[8])<<0) + (ord(self.serial_packet[9])<<8) + (ord(self.serial_packet[10])<<16) + (ord(self.serial_packet[11]) << 24)
        self.Infra = \
           [ord(self.serial_packet[12]), 
           ord(self.serial_packet[13]), 
           ord(self.serial_packet[14]), 
           ord(self.serial_packet[15]), 
           ord(self.serial_packet[16]), 
           ord(self.serial_packet[17])]
           
        #fill up the info to be published
        self.parsed_serial_data= BuddySerial(
            Stamp=rospy.Time.now(),
            Packetheader=self.Packetheader, 
            ValidData=self.ValidData,
            Ultra= [self.UltraF, self.UltraF],
            EncL=self.EncL, 
            EncR=self.EncR,
            Infra= [self.Infra[0], self.Infra[1], self.Infra[2], self.Infra[3], self.Infra[4], self.Infra[5]]
        )
        rospy.loginfo(self.parsed_serial_data)
        self.flg_rdy_to_pub = True
        return 0
 
    def print_data(self):
        """
        used for debug, print out the data
        """
        rospy.logdebug("DEBUG Packet Print - Buffer size: %s" %len(self.serial_packet))
        rospy.logdebug("Packet Header: %s, Valid Data: %s" %(self.Packetheader, self.ValidData))
        rospy.logdebug("EncoderL: %s, EncoderR: %s"%(self.EncL, self.EncR))
        rospy.logdebug("UltraF: %s, UltraB: %s"%(self.UltraF, self.UltraB))
        rospy.logdebug("Infra: "),
        rospy.logdebug(self.Infra)
        
def stringtofloat4(string):
    """
    4 character input
    little endian input
    """
    if len(string) != 4:
        rospy.logerr("ERROR: string %s is not 4 char long"%string)
        raise
        return -1
    else:
        return struct.unpack('>f', string[::-1])[0]
        
    return 0
  
if __name__=='__main__':
    rospy.logdebug("Trying to read from serial and publish (fake)? data")
    
    try:
        #node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None
        FakeSerial = serial_node(node_name='BuddySerial', serial_port_str='/dev/ttyUSB0', publish_rate = 100, DEBUG_EN=True)
        FakeSerial.startNode()
    except rospy.ROSInterruptException:
        pass
