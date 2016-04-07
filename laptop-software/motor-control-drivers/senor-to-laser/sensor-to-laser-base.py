#!/usr/bin/python
import random
import rospy

from sensor_msgs.msg import LaserScan

class sensor_to_laser_base:
    def __init__(self, node_name, node_topic, publish_rate, queue_size):
        """
        Function for initializing 
        """
        rospy.init_node(node_name)
        rospy.is_shutdown()
        self.rate = rospy.Rate(publish_rate)
        self.pub = rospy.Publisher(node_topic, LaserScan, queue_size=queue_size)

    def startPublishing(self):
        """
        start publishing the data
        """
        while not rospy.is_shutdown():
            (ranges, intensities) = self.getData()
            FakeLaserScan= LaserScan(ranges=ranges, intensities=intensities)
            self.pub.publish(FakeLaserScan)
            self.rate.sleep()

    def getData(self,num_samples=10):
    	"""
    	This getData function needs to be overloaded, else default it will work
    	"""
        ranges = [random.random()*10 for i in range(num_samples)]
        intensities = [1 for i in range(num_samples)]
        return ranges, intensities

if __name__=='__main__':
    try:
        LaserData('FakeLaserDataNode', 'FakeLaserTopic', 10, 10)
    except rospy.ROSInterruptException:
        pass

