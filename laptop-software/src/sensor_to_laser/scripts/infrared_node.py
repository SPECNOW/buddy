#!/usr/bin/python
import rospy

#from sensor_msg.msg import LaserScan

from sensor_to_laser_base import sensor_to_laser_base
from motor_control_drivers.msg import BuddySerial

#create subscriber

class infra_red_sensor(sensor_to_laser_base):
    """
    sensor_to_laser_base methods: __init__, startPublishing, getData
    """    
    def __init__(self, node_name, node_topic, publish_rate, queue_size, subscribe_to_topic_name=None):
        """
        Function to initialize things
        """
        if subscribe_to_topic_name is not None:
            rospy.Subscriber(subscribe_to_topic_name, BuddySerial, self.callback)
        sensor_to_laser_base.__init__(node_name, node_topic, publish_rate, queue_size)
        self.ranges = []
        self.intensities = []
    
    def getData(self,num_samples=10):
        """
        """
        return self.ranges, self.intensities

    def callback(self, data):
        """
        Callback function to use when data arrives
        """
        if data.ValidData & 0x08:
            self.ranges = [
                    data.Infra0,
                    data.Infra1,
                    data.Infra2,
                    data.Infra3,
                    data.Infra4,
                    data.Infra5
                ]
            self.intensities = [100 for dataPoint in self.ranges]

if __name__=='__main__':
    try:
        #subscribe to something for testing

        #ultrasonic publisher
        ultraSonic = ultra_sonic_base(
            "infraRedNode", 
            "infra_red_data", 
            10, 
            10, 
            "FakeSerial"
        )
        ultraSonic.startPublishing()
    except rospy.ROSInterruptException:
        pass


