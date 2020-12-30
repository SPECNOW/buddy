#!/usr/bin/python3
import random
import rospy

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
    def __init__(self, node_name, queue_size=5):
        """
        * initialize stuff 
        * create a publisher instance
        * each instance should be 
        """
        rospy.init_node(node_name)

        self.sensor_index = int(rospy.get_param("sensorIndex", "0"))
        self.valid_data_mask = int(rospy.get_param("validMask", 0x10)) # bit 4, UltraA
        self.number_of_datapoints = int(rospy.get_param("numberDataPoints", "10"))
        self.sensor_type = rospy.get_param('sensorType', 'Ultra')
        assert self.sensor_type in ["Ultra", "Infra"], "Did not recognize sensor type '{}', must be 'Ultra' or 'Infra'".format(self.sensor_type)

        self.sub = rospy.Subscriber(
            rospy.get_param("BuddySerialDataTopic", "BuddyPacket"),
            BuddySerial,
            self.processData
        )
        self.pub = rospy.Publisher(
            "{}{}".format(
                rospy.get_param("LaserDataOutputTopic", "LaserData"),
                self.sensor_index
            ),
            LaserScan,
            queue_size=queue_size
        )        
        return
    
    def processData(self, data):
        # Check if interested data has changed
        distance = 0
        if data.ValidData & self.valid_data_mask:
            # Extarct Data, convert Data, and Publish Data
            distance =  float(getattr(data, self.sensor_type)[self.sensor_index])/100 # cm -> m

            # Initialize Laser Data
            sensor_data = LaserScan(ranges = [0], intensities = [0] )
            sensor_data.header.stamp = rospy.Time.now()
            sensor_data.header.frame_id = 'frame_{}{}'.format(
                rospy.get_param("LaserDataOutputTopic", "LaserData"),
                self.sensor_index
            )
            sensor_data.angle_min = -float(rospy.get_param("sensorAngleRad", "0.31415692"))/2 #rad
            sensor_data.angle_max = float(rospy.get_param("sensorAngleRad", "0.31415692"))/2  #rad
            sensor_data.angle_increment = float(rospy.get_param("sensorAngleRad", "0.31415692"))/self.number_of_datapoints    # rad
            sensor_data.range_min = float(rospy.get_param("sensorMinimumDistance", "0"))  # m
            sensor_data.range_max = float(rospy.get_param("sensorMaximumDistance","5"))  # m
            
            sensor_data.ranges = [distance]*self.number_of_datapoints
            sensor_data.intensities = [1]*self.number_of_datapoints
            self.pub.publish(sensor_data)
        return

if __name__=='__main__':
    LaserPublisher = sensor_to_laser_base('laserScanDataNode', 10)
    rospy.spin()
