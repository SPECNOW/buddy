#!/usr/bin/python
import random
import rospy

"""
CP Aug 31, 2016

This doesn't work, but it looks roughly like what we
want it to look like.
"""
from sensor_msgs.msg import LaserScan
from motor_control_drivers.msg import BuddySerial

class sensor_manager:
    def __init__(self, number_of_sensors, node_name, node_topic, publish_rate, queue_size):
        """
        Function for initializing 
            * Subscribe to the BuddyMsg Topic
            * Create an array for Sensors to manage/instantiate them            	
        """
        rospy.init_node(node_name)
        rospy.is_shutdown()
        self.sensor_array = []
        for sensor_number in range(number_of_sensors):
            self.sensor_array.append(
                sensor_to_laser_base()
            )
        return

    def callback(self, data):
        """
            * Update releveant Managed sensors
        """
	for sensor in self.sensor_array:
            sensor.setData(data.Infra[self.sensor_array.index(sensor)])
        return

class sensor_to_laser_base:
    def __init__(self, node_topic, queue_size, publish_rate, number_of_datapoints=10):
        """
        * initialize stuff 
        """
        self.data = None
        self.number_of_datapoints = number_of_datapoints
        self.rate = rospy.Rate(publish_rate)
        self.pub = rospy.Publisher(node_topic, LaserScan, queue_size=queue_size)
        return

    def startPublishing(self):
        """
        start publishing the data
        """
        while not rospy.is_shutdown():
            (ranges, intensities) = self.getData()
            FakeLaserScan= LaserScan(ranges=ranges, intensities=intensities)
            self.pub.publish(FakeLaserScan)
            self.rate.sleep()
        return

    def setData(self, data):
        self.data = data
        return

    def getData(self,num_samples=10):
    	"""
    	This getData function needs to be overloaded, else default it will work
    	"""
        ranges = [random.random()*10 for i in range(self.number_of_datapoints)]
        intensities = [1 for i in range(self.number_of_datapoints)]
        return ranges, intensities

if __name__=='__main__':
    try:
        FakeLaser=sensor_to_laser_base('FakeLaserDataNode', 'FakeLaserTopic', 10, 10)
        FakeLaser.startPublishing()
    except rospy.ROSInterruptException:
        pass

