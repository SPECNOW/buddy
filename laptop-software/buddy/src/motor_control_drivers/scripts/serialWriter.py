#!/usr/bin/python3
import rospy
import serial

from motor_control_drivers.msg import BuddySerial
from motor_control_drivers.msg import BuddyCommand

"""
Simple Code to get data from Buddy over Serial:
* serialWriter does a writes received commands (could be Get Packet, could be motor speed) to the serial device
"""

buddy_serial =  serial.Serial(port=rospy.get_param("buddyPort", "/dev/ttyUSB0"), baudrate=115200, stopbits=1, parity='N', rtscts=0, timeout=0.1, bytesize=8)

def serialWriter(buddy):
    rospy.logdebug("Received Command [{} {}]".format(buddy.Command[0], buddy.Command[1]))
    buddy_serial.write([buddy.Command[0], buddy.Command[1]])
    pass


if __name__=="__main__":
    rospy.init_node('SerialWriter', anonymous=True)
    rospy.Subscriber('BuddyCommand', BuddyCommand, serialWriter)
    rospy.spin()