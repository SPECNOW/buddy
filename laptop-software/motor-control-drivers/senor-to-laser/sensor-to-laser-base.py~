#!/usr/bin/python
import random
import rospy

from sensor_msgs.msg import LaserScan

def LaserData(node_name, node_topic, publish_rate):
    rospy.init_node(node_name)
    rospy.is_shutdown()
    rate = rospy.Rate(publish_rate)
    pub = rospy.Publisher(node_topic, LaserScan, queue_size=5)
    while not rospy.is_shutdown():
        (ranges, intensities) = getData()
        FakeLaserScan= LaserScan(ranges=ranges, intensities=intensities)
        pub.publish(FakeLaserScan)
        rate.sleep()

def getData(num_samples=10):
    ranges = [random.random()*10 for i in range(num_samples)]
    intensities = [1 for i in range(num_samples)]
    return ranges, intensities

if __name__=='__main__':
    try:
        LaserData('FakeLaserDataNode', 'FakeLaserTopic', 10)
    except rospy.ROSInterruptException:
        pass

