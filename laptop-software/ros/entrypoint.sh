#!/bin/bash
source /opt/ros/kinetic/setup.bash
source /home/buddy/ros/devel/setup.bash
cd /home/buddy/ros/
catkin_make
sleep 10000
roslaunch src/buddy_launch.launch