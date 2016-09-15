#!/usr/bin/python
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
    def __init__(self, node_topic, queue_size, publish_rate, number_of_datapoints=10):
        """
        * initialize stuff 
	* create a publisher instance
        * each instance should be 
        """
	self.data_source_topic # = rospy.get_param(someParam)
	self.sensor_number # = rospy.get_param(someParam)
	self.publish_topic # = rospy.get_param(someParam) + sensor_number
	self.sensor_width_angle # = rospy.get_param(someParam)
	self.sensor_min_value # = rospy.get_param(someParam)
	self.sensor_max_value # = rospy.get_param(someParam)
	self.sensor_scale_factor # = rospy.get_param(someParam)
	self.valid_data_mask # = rospy.get_param(someParam)

        rospy.init_node(node_name)
	self.sub = rospy.Subscriber(data_source_topic, BuddySerial, proccessData)
        self.rate = rospy.Rate(publish_rate)
        self.pub = rospy.Publisher(publish_topic, LaserScan, queue_size=queue_size)
        # rospy.is_shutdown() # Does this do anything?
	return

    def startPublishing(self):
        """
        start publishing the data
        function call is blocking will not return till shutdown
        """
        while not rospy.is_shutdown():
            (ranges, intensities) = self.getData_laserScan()
            FakeLaserScan= LaserScan(ranges=ranges, intensities=intensities)
            self.pub.publish(FakeLaserScan)
            self.rate.sleep()
        return

    def getData_laserScan(self,num_samples=10):
    	"""
    	This getData function needs to be overloaded, else default it will work
    	"""
        ranges = [random.random()*10 for i in range(self.number_of_datapoints)]
        intensities = [1 for i in range(self.number_of_datapoints)]
        return ranges, intensities
    
    def processData(self, data):
        # CHeck if interested data has changed
	if data.ValidData && self.valid_data_mask:
	    # Extarct Data, convert Data, and Publish Data
	return

if __name__=='__main__':
    try:
        FakeLaser=sensor_to_laser_base('FakeLaserDataNode', 'FakeLaserTopic', 10, 10)
        FakeLaser.startPublishing()
    except rospy.ROSInterruptException:
        pass

