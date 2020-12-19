buddy
=====

ROS Buddy Serial Node

This package will read the Serial Data from the Hercules uC and then publish the data
to the approriate Topics. It will also write data to the Serial from some subscribed topics.

|========|
| Serial |
|========|

Publishes:
	- UltraSonic Sensor Array Data (as a ____ message type)
	- Infrared Sensor Array Data (as a ____ message type)
	- Motor Encoder Data (as a ____ message type) (subscribed to by Differential Drive Node)
	- GPS Data (?)

Subscribes:
	- Motor Power Levels (from Differential Drive Node)
	- Motor Brake Enable


