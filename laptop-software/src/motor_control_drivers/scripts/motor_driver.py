#!/usr/bin/python
import rospy
import sys
import struct
import string
import time

from geometry_msgs.msg import Twist
from motor_control_drivers.msg import BuddySerial

class motor_node:
    def __init__(self, node_name=None, subscription_names=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
        """
        publish rate in herts
        subscription name is the topic to which it subscribe to, from the data it will return to hercules, can be list
        no node name will create a fake node to publish random data
        
        This node once started, will read from serial. Will publish data as it arrives and it's valid flag is set
        """
        publish_rate = int(rospy.get_param("publishRate", "100.0"))
        publish_topic = rospy.get_param("motorTopicOut", "motor_power")
        self.DEBUG_EN = rospy.get_param("debugEnable", "True").upper() == "TRUE"
        
        self.flg_rdy_to_pub = False #flag if data is valid to pubish
        self.rosRate = None         #obj for the rospy.Rate()
        self.times = []
        self.positions = {'left':[], 'right':[]}
        
        if publish_rate is None: 
            publish_rate = 100
        if queue_size is None: 
            queue_size = 10.0

        if subscription_names is not None and isinstance(subscription_names, list): # if it is passed and a list
            rospy.logdebug("Subscribed to: ")
            for i in subscription_names:
                rospy.logdebug(i)
                self.sub = rospy.Subscriber(i, BuddySerial, self.processData)
        elif subscription_names is not None and isinstance(subscription_names, basestring): # if it is passed and just a string
            rospy.logdebug("Subscribed to: %s"%subscription_names)
            self.sub = rospy.Subscriber(subscription_names, BuddySerial, self.processData)
        else:
            rospy.logdebug("Not subscribing to anything")

        #create publisher
        rospy.init_node(node_name, log_level=rospy.DEBUG if self.DEBUG_EN else rospy.INFO)
        rospy.is_shutdown()
        self.pub=rospy.Publisher(publish_topic, Twist, queue_size=queue_size)
        self.rosRate=rospy.Rate(publish_rate)        

    def processData(self, data):
        self.times.append(data.Stamp.secs+data.Stamp.nsecs*1e-9)
        self.positions['left'].append(data.EncL)
        self.positions['right'].append(data.EncR)
        if len(self.times) == 3:
            self.times.pop(0)
            self.positions['left'].pop(0)
            self.positions['right'].pop(0)
            rospy.logdebug("Left Velocty: Position: %s, %s, delat: %s Time: %s %s delta: %s" % (str(self.positions['left'][1]), str(self.positions['left'][0]), str(self.positions['left'][1]-self.positions['left'][0]), str(self.times[1]), str(self.times[0]), str(self.times[1] - self.times[0])) )
            rospy.loginfo("Left Velocty: %s" % str( (self.positions['left'][1]-self.positions['left'][0])/(self.times[1]-self.times[0]) ) )
            #rospy.loginfo("Right Velocty: %s" % str( (self.positions['right'][1]-self.positions['right'][0])/(self.times[1]-self.times[0]) ) )
        return

    def startNode(self):
        """
        starts the node running to receive and send out data by publishing and subscribing
        """
        while not rospy.is_shutdown():
            if self.flg_rdy_to_pub:
                self.pub.publish(Twist())
            else:
                pass
                # rospy.logdebug("Not ready to publish")
            self.rosRate.sleep()
  
if __name__=='__main__':    
    try:
        #node_name=None, subscription_names=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None
        motor_driver = motor_node(node_name='BuddyMotorController', publish_rate = 100, DEBUG_EN=True, subscription_names="buddySerial")
        motor_driver.startNode()
    except rospy.ROSInterruptException:
        pass
