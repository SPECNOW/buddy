#!/usr/bin/python
import random
import rospy
import tf

"""
CP Aug 31, 2016
updated sept 14th
This doesn't work, but it looks roughly like what we
want it to look like.

Design:
1) use launch file to launch X number copies of this file with params to specify which sensor etc etc
"""
from sensor_msgs.msg import LaserScan
from motor_control_drivers.msg import BuddySerial

class sensor_to_laser_base:
    def __init__(self, node_name, queue_size=5, publish_rate=10):
        """
        * initialize stuff 
        * create a publisher instance
        * each instance should be 
        """
        rospy.init_node(node_name)
        self.rate = rospy.Rate(publish_rate)
        
        self.data_source_topic = rospy.get_param("topicIn", "/Fake_serial_data_topic")
        self.sensor_number = int(rospy.get_param("sensorNum", "99"))
        self.data_dest_topic = rospy.get_param("topicOut", "Fake_sensor_data") + str(self.sensor_number)
        self.sensor_scale_factor = float(rospy.get_param("sensorScale", '0.02'))
        self.valid_data_mask = int(rospy.get_param("validMask", "16")) # bit 4, UntraA
        self.sensor_type = rospy.get_param("sensorType","Ultra")
        self.number_of_datapoints = int(rospy.get_param("numberDataPoints","10"))

        sensor_width_angle = float(rospy.get_param("sensorAngleRad", "0.31415692"))
        sensor_min_value = float(rospy.get_param("sensorMinDistM", "0")) #in meters
        sensor_max_value = float(rospy.get_param("sensorMaxDistM","5")) # in meters

        self.sub = rospy.Subscriber(self.data_source_topic, BuddySerial, self.processData)
        self.pub = rospy.Publisher(self.data_dest_topic, LaserScan, queue_size=queue_size)
        
        self.sensor_LaserScan = LaserScan(ranges = [0], intensities = [0] )
        self.sensor_LaserScan.header.frame_id = 'frame_' + self.data_dest_topic
        self.sensor_LaserScan.angle_min = -sensor_width_angle/2 #rad
        self.sensor_LaserScan.angle_max = sensor_width_angle/2  #rad
        self.sensor_LaserScan.angle_increment = sensor_width_angle/self.number_of_datapoints    # rad
        self.sensor_LaserScan.range_min = sensor_min_value  # m
        self.sensor_LaserScan.range_max = sensor_max_value  # m
        # rospy.is_shutdown() # Does this do anything?
        return

    def startPublishing(self):
        """
        start publishing the data
        function call is blocking will not return till shutdown
        """
        while not rospy.is_shutdown():
            self.pub.publish(self.sensor_LaserScan)
            self.rate.sleep()
        return
    
    def processData(self, data):
        # Check if interested data has changed
        self.sensor_LaserScan.header.stamp = rospy.Time.now()
        distance = 0
        if data.ValidData & self.valid_data_mask:            
            # Extarct Data, convert Data, and Publish Data
            if self.sensor_type.lower() == "ultra".lower():
                distance =  float(ord(data.Ultra[self.sensor_number]))*self.sensor_scale_factor
            else:
                distance =  float(ord(data.Infra[self.sensor_number]))*self.sensor_scale_factor
            self.sensor_LaserScan.ranges = [distance]*self.number_of_datapoints
            self.sensor_LaserScan.intensities = [1]*self.number_of_datapoints
        else:
            pass
        return

if __name__=='__main__':
    try:
        FakeLaser=sensor_to_laser_base('laserScanDataNode', 1, 10)
        FakeLaser.startPublishing()
    except rospy.ROSInterruptException:
        pass