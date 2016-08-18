#!/usr/bin/python
import rospy

#from sensor_msg.msg import LaserScan

imoport sensor_to_laser_base
from motor_control_drivers.msg import BuddySerial

print "Ultrasonic node"


#create subscriber

class ultra_sonic_base(sensor_to_laser_base.sensor_to_laser_base):
    """
    sensor_to_laser_base methods: __init__, startPublishing, getData
    """

    """
    def __init__(self, node_name, subscribe_to_topic_name=None):
        """
        Function to initialize things
        """
        rospy.init_node(node_name, anonymous=True)
        if subscribe_to_topic_name is not None:
            rospy.Subscriber(subscribe_to_topic_name, 
    """

    def convertData_char2int(self):
        """
        convert chars to ints
        """
        
    def startReceiveData(self):
        """
        Function to call if we want to start receiving
        """

    def callback(self):
        """
        Callback function to use when data arrives
        """
    
    def getData(self, num_samples=10):
        """
        Overload the original object method
        """


if __name__=='__main__':
    try:
        #subscribe to something for testing
        rospy.init_node("ultraA", anonymous=True)
        rospy.Subscriber("FakeSerial", BuddySerial, callback)

        #ultrasonic publisher
        ultra_obj=ultra_sonic_base(ultrasonic_node, ultrasonic_topic, 10, 10)
        ultra_obj.startPublishing()


