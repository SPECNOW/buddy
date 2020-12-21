
FROM osrf/ros:kinetic-desktop-full

ARG USER=buddy

RUN useradd -ms /bin/bash $USER

RUN apt update -y 
RUN apt install -y python-rosdep python-wstool build-essential python-pip
RUN apt install -y python-rosinstall python-rosinstall-generator ros-kinetic-navigation ros-kinetic-slam-gmapping
RUN python -m pip install serial

COPY ./buddy/.catkin_workspace /home/$USER/ros/.catkin_workspace
COPY ./buddy/src /home/$USER/ros/src/

RUN chown -R $USER /home/$USER/ros
USER $USER
RUN /bin/bash -c 'source /opt/ros/kinetic/setup.bash && cd /home/$USER/ros && catkin_make'

WORKDIR /home/$USER/ros
COPY ./ros/entrypoint.sh .
#ENTRYPOINT roslaunch buddy_launch.launch
CMD ["/bin/bash", "./entrypoint.sh"]