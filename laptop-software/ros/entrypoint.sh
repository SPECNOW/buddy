#!/bin/bash
cd /home/buddy/ros/src/
echo "CURENT DIR: $(pwd)"
echo "ENV: $ROS_PACKAGE_PATH"
sleep 10
sleep 10000
roslaunch buddy_launch.launch