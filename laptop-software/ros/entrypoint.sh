#!/bin/bash
source /opt/ros/kinetic/setup.bash
source /home/$USER/ros/devel/setup.bash
cd /home/$USER/ros/
catkin_make
roslaunch src/buddy_launch.launch