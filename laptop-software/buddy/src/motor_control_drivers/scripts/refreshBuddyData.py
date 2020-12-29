#!/usr/bin/python3
import rospy

from motor_control_drivers.msg import BuddyCommand

"""
Simple Code to get data from Buddy over Serial:
* refreshBuddyData function sends a Get Packet request to the /BuddyCommand topic at a regular interval
** Relies on other ROS nodes to actually send the request over serial
"""

def refreshBuddyData():
    serialPublisher = rospy.Publisher("BuddyCommand", BuddyCommand, queue_size=10)
    rospy.loginfo("RATE: {}".format(rospy.get_param("refreshRate", 20)))
    rate = rospy.Rate(int(rospy.get_param("refreshRate", 20)))
    while not rospy.is_shutdown():
        cmd = BuddyCommand(Command=[ord('\xFF'), ord('\xFF')])
        serialPublisher.publish(cmd)
        rospy.logdebug("Sending get data request to Buddy")        
        rate.sleep()
    return

if __name__=="__main__":
    rospy.init_node('refreshBuddyData', anonymous=True)
    refreshBuddyData()