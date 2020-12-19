#!/usr/bin/python
import rospy
import sys
import struct
import string
import time
import random

from std_msgs.msg import UInt32
from std_msgs.msg import UInt32MultiArray

MAX_POSITION = int("0xffffffff",16)


# node that will publish motor power data to the serial node to write out to micro controller
# node that will subscribe to outputs of differencial_drive http://wiki.ros.org/differential_drive
# 
class power_writer_node:
    def __init__(self, node_name=None, left_motor_in=None, right_motor_in=None, publish_name=None, publish_rate=None, queue_size=None):
        
        if node_name is None:
            node_name=(rospy.get_param("nodeName", "ROS_Launch_default_motor_power_node"))
        if left_motor_in is None:
            left_motor_in=(rospy.get_param("leftMotorIn", "Not_subscribed"))
        if right_motor_in is None:
            right_motor_in=(rospy.get_param("rightMotorIn", "Not_subscribed"))
        if publish_name is None:
            publish_name=(rospy.get_param("publishTo", "ROS_Launch_default_motor_power"))
        if publish_rate is None:
            publish_rate=int(rospy.get_param("publishRate", "1.0"))
        if queue_size is None:
            queue_size=int(rospy.get_param("queueSize", "5"))
        
        self.publish_fake=bool(rospy.get_param("publishFake",0))
        self.motor_control_value = UInt32MultiArray(data=[0,0])
        
        self.flg_rdy_to_pub=None
        self.rosRate=rospy.Rate(publish_rate)
        
        if left_motor_in is not None:
            rospy.logdebug("Subscribed to: %s", left_motor_in)
        else:
            rospy.logdebug("Not subscribing to anything")
            
        #create publisher
        if self.publish_fake:
            rospy.loginfo("Creating fake node and publish fake data")
            rospy.init_node("Fake_serial_node", log_level=rospy.DEBUG)
            rospy.is_shutdown()
        else:
            rospy.init_node(node_name, log_level=rospy.DEBUG)
            rospy.is_shutdown()
            
        self.pub=rospy.Publisher(publish_name, UInt32MultiArray, queue_size=queue_size)
        self.sub_l = rospy.Subscriber(left_motor_in, UInt32, self.processData)
        self.sub_r = rospy.Subscriber(right_motor_in, UInt32, self.processData)
        
    def startPublishing(self):
        """
        start the node running to receive left and right motor velocity 
        """
        while not rospy.is_shutdown():
            if self.publish_fake: #generate fake data
                self.motor_control_value.data=[random.randrange(1000000),random.randrange(1000000)]
           
            rospy.logdebug("Data: "+ self.motor_control_value)

            if self.flg_rdy_to_pub:
                self.pub.publish(outputData)
            else:
                rospy.logdebug("Not ready to publish")
            self.rosRate.sleep()
            
    def processData(self, data):
        """
        take the data and process it
        differenciate left and right motor, convert motor power to control values to be published
        """
  
if __name__=='__main__':    
"""
code comes here when python file is random
can try to create a fake pulisher
"""
    
 """  
    def __init__(self, node_name=None, left_motor_ins=None, publish_rate=None, queue_size=None, DEBUG_EN=None):
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
            queue_size = 5

        if left_motor_ins is not None and isinstance(left_motor_ins, list): # if it is passed and a list
            rospy.logdebug("Subscribed to: ")
            for i in left_motor_ins:
                rospy.logdebug(i)
                self.sub = rospy.Subscriber(i, BuddySerial, self.processData)
        elif left_motor_ins is not None and isinstance(left_motor_ins, basestring): # if it is passed and just a string
            rospy.logdebug("Subscribed to: %s"%left_motor_ins)
            self.sub = rospy.Subscriber(left_motor_ins, BuddySerial, self.processData)
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
            velocity_left = calculate_velocity(
                self.positions['left'][1],
                self.positions['left'][0],
                self.times[1],
                self.times[0]
            )
            velocity_right = calculate_velocity(
                self.positions['right'][1],
                self.positions['right'][0],
                self.times[1],
                self.times[0]
            )
            # rospy.logdebug("Left Velocty: Position: %s, %s, delta: %s Time: %s, %s delta: %s" % self.positions[1], self.positions[0], delta_p, self.time[1], self.time[0], delta_t_left)
            rospy.loginfo("Left Velocty: %s" % velocity_left )
            #rospy.loginfo("Right Velocty: %s" % velocity_right )
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
        #node_name=None, left_motor_ins=None, serial_port_str=None, publish_rate=None, queue_size=None, DEBUG_EN=None
        motor_driver = motor_node(node_name='BuddyMotorController', publish_rate = 100, DEBUG_EN=True, left_motor_ins="buddySerial")
        motor_driver.startNode()
    except rospy.ROSInterruptException:
        pass
        
"""
