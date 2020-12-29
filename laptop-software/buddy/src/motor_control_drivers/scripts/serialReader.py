#!/usr/bin/python3
import rospy
import serial
import struct
import time

from motor_control_drivers.msg import BuddySerial

"""
Simple Code to get data from Buddy over Serial:
* processBuddyPacket reads Get Packet response from serial device and published result to /BuddyPacket
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
        assert len(data) == 18, "Incorrect Length"
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

buddy_serial =  serial.Serial(port=rospy.get_param("buddyPort", "/dev/ttyUSB0"), baudrate=115200, stopbits=1, parity='N', rtscts=0, timeout=0.1, bytesize=8)

def proccessBuddyPacket():
    buddyPublisher = rospy.Publisher("BuddyPacket", BuddySerial, queue_size=10)
    while not rospy.is_shutdown():
        while buddy_serial.in_waiting < 18:
            time.sleep(0.000001)
        try:
            packet = BuddyPacket(buddy_serial.read(18))
            buddyData = BuddySerial(
                Packetheader= packet.header,
                ValidData = packet.validData,
                Ultra = [packet.ultraSonicFront, packet.ultraSonicBack],
                EncL = packet.encoderLeft,
                EncR = packet.encoderRight,
                Infra = packet.infra
            )        
            buddyPublisher.publish(buddyData)
        except Exception as e:
            rospy.loginfo("Error: {}".format(e))
            pass
    return


if __name__=="__main__":
    rospy.init_node('SerialReader', anonymous=True)
    proccessBuddyPacket()