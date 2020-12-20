
FROM osrf/ros:kinetic-desktop-full

RUN useradd -ms /bin/bash buddy

RUN apt update -y 
RUN apt install -y python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential python-pip
RUN python -m pip install serial

COPY ./buddy/.catkin_workspace /home/buddy/ros/.catkin_workspace
COPY ./buddy/src /home/buddy/ros/src/
RUN chown -R buddy /home/buddy/ros
USER buddy
#RUN chmod -R +w /home/buddy/ros/src

RUN git clone https://github.com/ros-perception/openslam_gmapping.git /home/buddy/ros/src/openslam_gmapping
RUN git clone https://github.com/ros-perception/slam_gmapping.git /home/buddy/ros/src/slam_gmapping
RUN /bin/bash -c 'source /opt/ros/kinetic/setup.bash && cd /home/buddy/ros && catkin_make'
COPY ./ros/entrypoint.sh /home/buddy/

WORKDIR /home/buddy/ros

#ENTRYPOINT roslaunch buddy_launch.launch
CMD ["/bin/bash", "/home/buddy/entrypoint.sh"]