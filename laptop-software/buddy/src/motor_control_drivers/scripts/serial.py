#!/usr/bin/python
import rospy
import serial

from motor_control_drivers.msg import BuddySerial
from motor_control_drivers.msg import BuddyCommand

"""
Simple Code to get data from Buddy over Serial:
* getBuddyPacket function sends a Get Packet request to the /BuddyCommand topic and waits
* serialWriter does a writes received commands (could be Get Packet, could be motor speed) to the serial device
* getBuddyPacket reads Get Packet response from serial device and published result to /BuddyPacket
"""

class BuddyPacket:
    header=\
    validData=\
    encoderLeft=\
    encoderRight=\
    ultraSonicFront=\
    ultraSonicBack = None
    infraRed=[]

    def __init__(self, data):
        assert len(data) == 18
        self.header = data[0]
        self.validData = data[1]
        self.ultraSonicFront = data[2]
        self.ultraSonicBack = data[3]
        self.encoderLeft = struct.unpack('!f', data[4:8])[0]
        self.encoderRight = struct.unpack('!f', data[8:12])[0]
        self.infra = [
            data[12],
            data[13],
            data[14],
            data[15],
            data[16],
            data[17]
        ]

    def __str__(self):
        return """
Header:         {}
ValidData:      {}
UltraSonic:
    front:     {}
    back:      {}
Encoder:
    left:    {}
    right:   {}
InfraRed:
    0:   {}
    1:   {}
    2:   {}
    3:   {}
    4:   {}
    5:   {}""".format(
        self.header,
        self.validData,
        self.ultraSonicFront,
        self.ultraSonicBack,
        self.encoderLeft,
        self.encoderRight,
        self.infra[0],
        self.infra[1],
        self.infra[2],
        self.infra[3],
        self.infra[4],
        self.infra[5])

buddy_serial =  serial.Serial(port='/dev/ttyUSB0', baudrate=115200, stopbits=1, parity='N', rtscts=0, timeout=0.1, bytesize=8)

def getBuddyPacket():
    serialPublisher = rospy.Publisher("BuddyCommand", BuddyCommand, queue_size=10)
    buddyPublisher = rospy.Publisher("BuddyPacket", BuddySerial, queue_size=10)
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        cmd = BuddyCommand(Command=[ord('\xFF'), ord('\xFF')])
        serialPublisher.publish(cmd)
        rospy.loginfo("Publishing {} {}".format(cmd.Command[0], cmd.Command[1]))        
        rate.sleep()
        try:
            packet = BuddyPacket(serial.read(100))
            buddyData = BuddySerial(
                Packetheader= packet.header,
                ValidData = packet.validData,
                Ultra = [packet.ultraSonicFront, packet.ultraSonicBack],
                EncL = packet.encoderLeft,
                EncR = packet.encoderRight,
                Infra = packet.infra
            )        
            buddyPublisher.publish(buddyData)
        except:
            pass
    return

def serialWriter(buddy):
    rospy.loginfo("Received {} {}".format(buddy.Command[0], buddy.Command[1]))
    buddy_serial.write([buddy.Command[0], buddy.Command[1]])
    pass


if __name__=="__main__":
    rospy.init_node('Serial', anonymous=True)
    rospy.Subscriber('BuddyCommand', BuddyCommand, serialWriter)
    getBuddyPacket()