#!/usr/bin/python
import random
import rospy
import serial
import sys

class serial_node:
    def __init__(self, node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        """
        publish rate in herts
        subscription name is the topic to which it subscribe to, from the data it will return to hercules, can be list
        no node name will create a fake node to publish random data
        
        This node once started, will read from serial. Will publish data as it arrives and it's valid flag is set
        """
        self.serial_buffer=''
        self.serial_packet=''
        self.flg_rdy_to_pub=False #flag if data is valid to pubish
        self.DEBUG_EN=DEBUG_EN
        
        self.Packetheader = ord(serial_packet[0])
        self.ValiData = ord(serial_packet[1])
        self.EncL = stringtofloat(serial_packet[2:5])
        self.EncR = stringtofloat(serial_packet[6:9])
        self.UltraF = ord(serial_packet[10])
        self.UltraB = ord(serial_packet[11])
        self.infra = [ord(serial_packet[12]), 
           ord(serial_packet[13]), 
           ord(serial_packet[14]), 
           ord(serial_packet[15]), 
           ord(serial_packet[16]), 
           ord(serial_packet[17])]

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
            
        if node_name is None or serial_port_str is None: #creation of the node
            print("Creating fake node and publish fake data")
        else:
            ser=initSerial()  #initialize serial port
            if ser is None:
                sys.exit("ERROR with serial port")
    
    def startNode(self):
        """
        starts the node running to receive and send out data by publishing and subscribing
        """
        while True and self.DEBUG_EN:
            self.readSerial2Buffer(self.ser)
            print(self.serial_buffer)
            self.clearSerialBuffer()
            
            ##get data
            ##compile pkt
            #publish
            #sleep
    
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
    
 
    def pkt_process_and_publish(self):
        """
        check if buffer has more than 18 bytes, then sort bytes into strings, then then publish
        """
        if len(self.serial_buffer) >= 18:
            self.serial_packet = self.serial_buffer.pop(18)
        self.Packetheader = ord(serialbuffer[0])
        self.ValiData = ord(serialbuffer[1])
        self.EncL = stringtofloat(serialbuffer[2:5])
        self.EncR = stringtofloat(serialbuffer[6:9])
        self.UltraF = ord(serialbuffer[10])
        self.UltraB = ord(serialbuffer[11])
        self.infra = [ord(serialbuffer[12]), 
           ord(serialbuffer[13]), 
           ord(serialbuffer[14]), 
           ord(serialbuffer[15]), 
           ord(serialbuffer[16]), 
           ord(serialbuffer[17])]
 
def stringtofloat(string):
  return float(ord(string[0])<< 3 + ord(string[1]) << 2 + ord(string[2]) << 1 + ord(string[3]))
  
if __name__=='__main__':
    print("Trying to read from serial and publish (fake) data")
    
    try:
    #self, node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        FakeSerial=serial_node(node_name='FakeSerial', serial_port_str='ttyUSB0', DEBUG_EN=True)
        FakeSerial.startNode()
    except rospy.ROSInterruptException:
        pass
